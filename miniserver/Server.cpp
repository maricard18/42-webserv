/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 12:51:47 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/10/16 16:29:40 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cerrno>
#include <cstring>
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
	: CommonDirectives("/var/www/html", false),
	  _address(ip_to_in_addr_t("0.0.0.0")),
	  _listen(0),
	  _clientMaxBodySize(1000)
{
}

Server::Server(const std::string& root,
			   const std::vector<std::string>& index,
			   u_int16_t listen)
	: CommonDirectives(root, index, false),
	  _address(ip_to_in_addr_t("0.0.0.0")),
	  _listen(listen),
	  _clientMaxBodySize(1000)
{
}

Server::Server(const Server& value)
	: CommonDirectives(value._root, value._index, value._autoindex),
	  _serverNames(value._serverNames),
	  _address(value._address),
	  _listen(value._listen),
	  _clientMaxBodySize(value._clientMaxBodySize),
	  _errorPage(value._errorPage),
	  _locations(value._locations)
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

in_addr_t Server::getAddress() const
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
	std::string buf;
	std::stringstream ss(value);

	while (ss >> buf)
	{
		if (*buf.begin() != '.' && *buf.end() != '.')
			this->_serverNames.push_back(buf);
		else
			return (1);
	}
	return (0);
}

int Server::setAddress(const std::string& value)
{
	if (!(this->_address = ip_to_in_addr_t(value)))
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

void Server::run()
{
	const int trueFlag = 1;

	// to get an non block socket use <SOCK_STREAM | SOCK_NONBLOCK> as second argument
	if ((this->_socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) < 0)
	{
		std::cerr << "Error creating socket." << std::endl
				  << "errno: " << errno << std::endl;
		return;
	}
	if (setsockopt(this->_socket,
				   SOL_SOCKET,
				   SO_REUSEADDR,
				   &trueFlag,
				   sizeof(int)) < 0)
	{
		std::cerr << "Error setting up socket." << std::endl
				  << "errno: " << errno << std::endl;
		return;
	}
	bzero(&this->_serverAddress, sizeof(this->_serverAddress));
	this->_serverAddress.sin_family = AF_INET;
	this->_serverAddress.sin_addr.s_addr = htonl(this->getAddress());
	this->_serverAddress.sin_port = htons(this->getListenPort());
	std::cout << "SERVER PORT = " << this->getListenPort() << std::endl;

	if (bind(this->_socket,
			 (struct sockaddr*)&this->_serverAddress,
			 sizeof(this->_serverAddress)) < 0)
	{
		std::cerr << "Error binding socket." << std::endl
				  << "errno: " << errno << std::endl;
		return;
	}

	if (listen(this->_socket, 1) < 0)
	{
		std::cerr << "Error listening socket." << std::endl
				  << "errno: " << errno << std::endl;
		return;
	}
}