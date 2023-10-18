/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 12:51:47 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/10/18 18:00:12 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

static in_addr_t ip_to_in_addr_t(const std::string& ip_address)
{
	in_addr_t result = 0;
	int octet;
	char delimiter;
	std::istringstream ss(ip_address);

	for (int i = 0; i < 4; i++)
	{
		if (!(ss >> octet) || octet < 0 || octet > 255)
		{
			// Invalid octet or format
			return (0);
		}
		if (i < 3 && !(ss >> delimiter) && delimiter != '.')
		{
			// Invalid delimiter
			return (0);
		}
		result = (result << 8) | static_cast<in_addr_t>(octet);
	}

	return (result);
}

Server::Server()
	: CommonDirectives("/var/www/html"),
	  _address("0.0.0.0"),
	  _listen(8080),
	  _clientMaxBodySize(1000),
	  _socket(0),
	  _serverAddress(sockaddr_in())
{
}

Server::Server(const Server& value)
	: CommonDirectives(value._root, value._index, value._autoindex),
	  _serverNames(value._serverNames),
	  _address(value._address),
	  _listen(value._listen),
	  _clientMaxBodySize(value._clientMaxBodySize),
	  _errorPage(value._errorPage),
	  _locations(value._locations),
	  _socket(value._socket),
	  _serverAddress(value._serverAddress)
{
}

Server& Server::operator=(const Server& value)
{
	if (this == &value)
		return (*this);

	Server tmp(value);

	std::swap(*this, tmp);
	return (*this);
}

Server::~Server()
{
	for (
		std::map<std::string, Location*>::iterator
			it = this->_locations.begin();
		it != this->_locations.end(); ++it)
		delete it->second;
}

std::vector<std::string> Server::getServerNames() const
{
	return (this->_serverNames);
}

std::string Server::getAddress() const
{
	return (this->_address);
}

u_int16_t Server::getListenPort() const
{
	return (this->_listen);
}

u_int32_t Server::getClientMaxBodySize() const
{
	return (this->_clientMaxBodySize);
}

std::string Server::getErrorPage(int error_code)
{
	return (this->_errorPage[error_code]);
}

Location& Server::getLocation(const std::string& location)
{
	return (*this->_locations[location]);
}

int32_t Server::getSocket() const
{
	return (this->_socket);
}

const sockaddr_in& Server::getServerAddress() const
{
	return (this->_serverAddress);
}

std::string Server::getUploadStore() const
{
	return (this->_uploadStore);
}

int Server::setServerNames(const std::string& value)
{
	std::vector<std::string> serverNames;
	std::stringstream ss(value);
	std::string domain;

	while (ss >> domain)
	{
		if (*domain.begin() != '.' && *domain.end() != '.')
			serverNames.push_back(domain);
		else
			return (1);
	}
	this->_serverNames = serverNames;
	return (0);
}

int Server::setAddress(const std::string& value)
{
	if (!ip_to_in_addr_t(value))
		return (1);
	this->_address = value;
	return (0);
}

int Server::setListen(const std::string& value)
{
	int port;
	std::string buf;
	std::stringstream ss(value);

	std::getline(ss, buf, ':');
	if (!buf.empty())
		this->setAddress(buf);
	ss >> port;
	this->_listen = port;
	if (ss >> buf) // check if it has more text
		return (1);
	return (0);
}

int Server::setClientMaxBodySize(const std::string& value)
{
	if (value.find_first_not_of("0123456789kKmM") != std::string::npos)
		return (1);
	std::string unit;
	std::stringstream ss(value);

	ss >> this->_clientMaxBodySize;
	if (this->_clientMaxBodySize == 0)
		return (1);
	ss >> unit;
	if (unit == "k" || unit == "K")
		this->_clientMaxBodySize *= 1000;
	else if (unit == "m" || unit == "M")
		this->_clientMaxBodySize *= 1000000;
	if (ss >> unit) // check if it has more text
		return (1);
	return (0);
}

int Server::setErrorPage(const std::string& value)
{
	int error;
	std::string file;
	std::stringstream ss(value);

	ss >> error;
	ss >> file;
	if (file.find_first_of('.') == std::string::npos ||
		file.find_first_of('.') != file.find_last_of('.') ||
		*file.begin() != '/' || *file.end() != '.') // check if is path
		return (1);
	this->_errorPage[error] = file;
	if (ss >> file) // check if it has more text
		return (1);
	return (0);
}

int Server::setLocation(const std::string& dir, const Location& value)
{
	if ((this->_locations[dir] = new Location(value)))
		return (0);
	return (1);
}

int Server::setUploadStore(const std::string& value)
{
	std::stringstream ss(value);
	std::string dir;

	ss >> dir;
	if (dir.at(0) != '/') // check if is path
		return (1);
	this->_uploadStore = dir;
	if (ss >> dir) // check if it has more text
		return (1);
	return (0);
}

int Server::run()
{
	const int trueFlag = 1;
	std::stringstream port;
	port << this->_listen;

	// to get an non block socket use <SOCK_STREAM | SOCK_NONBLOCK> as second argument
	if ((this->_socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) < 0)
	{
		std::stringstream ss;
		ss << errno;
		MESSAGE("socket(): " + ss.str() + ": " + (std::string)strerror(errno),
				CRITICAL);
		return (1);
	}
	if (setsockopt(this->_socket,
				   SOL_SOCKET,
				   SO_REUSEADDR,
				   &trueFlag,
				   sizeof(int)) < 0)
	{
		std::stringstream ss;
		ss << errno;
		MESSAGE(
			"setsockopt(): " + ss.str() + ": " + (std::string)strerror(errno),
			CRITICAL);
		return (1);
	}
	bzero(&this->_serverAddress, sizeof(this->_serverAddress));
	this->_serverAddress.sin_family = AF_INET;
	this->_serverAddress.sin_addr.s_addr =
		htonl(ip_to_in_addr_t(this->getAddress()));
	this->_serverAddress.sin_port = htons(this->getListenPort());

	if (bind(this->_socket,
			 (struct sockaddr*)&this->_serverAddress,
			 sizeof(this->_serverAddress)) < 0)
	{
		std::stringstream ss;
		ss << errno;
		MESSAGE("bind(): " + ss.str() + ": " + (std::string)strerror(errno),
				CRITICAL);
		return (1);
	}

	if (listen(this->_socket, 1) < 0)
	{
		std::stringstream ss;
		ss << errno;
		MESSAGE("listen(): " + ss.str() + ": " + (std::string)strerror(errno),
				CRITICAL);
		return (1);
	}
	return (0);
}

void Server::stop()
{
	if (this->_socket)
		close(this->_socket);
	this->_socket = 0;
}
