/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 12:51:47 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/10/31 16:52:29 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::map<std::string, int (Server::*)(const std::string&)> Server::_methods;

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
	: CommonDirectives(),
	  _address("0.0.0.0"),
	  _listen(8080),
	  _clientMaxBodySize(1000),
	  _socket(0),
	  _serverAddress(sockaddr_in())
{
	Server::initializeMethods();
}

Server::Server(const Server& value)
	: CommonDirectives(value._root,
					   value._index,
					   value._uploadStore,
					   value._autoindex),
	  _serverNames(value._serverNames),
	  _address(value._address),
	  _listen(value._listen),
	  _clientMaxBodySize(value._clientMaxBodySize),
	  _errorPage(value._errorPage),
	  _socket(value._socket),
	  _serverAddress(value._serverAddress)
{
	for (
		std::map<std::string, Location*>::const_iterator
			it = value._locations.begin();
		it != value._locations.end(); ++it)
	{
		this->_locations[it->first] = new Location(*it->second);
	}
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
	{
		delete it->second;
		it->second = 0;
	}
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

Location* Server::getLocation(const std::string& location)
{
	if (this->_locations[location])
		return (this->_locations[location]);
	return (NULL);
}

int32_t Server::getSocket() const
{
	return (this->_socket);
}

const sockaddr_in& Server::getServerAddress() const
{
	return (this->_serverAddress);
}

int Server::setServerNames(const std::string& value)
{
	std::vector<std::string> serverNames;
	std::stringstream ss(value);
	std::string domain;

	while (ss >> domain)
	{
		if (!domain.empty() && domain.find_first_of('.') != std::string::npos &&
			*domain.begin() != '.' && *domain.end() != '.')
			serverNames.push_back(domain);
	}
	if (serverNames.empty())
		return (1);
	this->_serverNames = serverNames;
	return (0);
}

int Server::setAddress(const std::string& value)
{
	if (std::count(value.begin(), value.end(), '.') != 3)
		return (1);
	if (!ip_to_in_addr_t(value))
		return (1);
	this->_address = value;
	return (0);
}

int Server::setListen(const std::string& value)
{
	int port = -1;
	std::string address;
	std::string buf;
	std::stringstream ss(value);

	if (ss.str().find(':') != std::string::npos)
	{
		std::getline(ss, buf, ':');
		std::stringstream val(buf);
		val >> address;
		if (!address.empty() && this->setAddress(address) &&
			address != "0.0.0.0")
		MESSAGE(address + ": Invalid address, using 0.0.0.0", WARNING);
	}
	std::getline(ss, buf, ':');
	std::stringstream val(buf);
	val >> port;
	if (port <= 0 || port > std::numeric_limits<u_int16_t>::max() ||
		val.str().find_first_not_of(" \t0123456789") != std::string::npos)
		return (1);
	this->_listen = port;
	if (ss >> buf) // check if it has more text
		return (1);
	return (0);
}

int Server::setClientMaxBodySize(const std::string& value)
{
	if (value.find_first_not_of(" \t0123456789kKmM") != std::string::npos)
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
	if (file.empty() || file.find_first_of(" \r\n\t") != std::string::npos ||
		file.find_first_of('.') == std::string::npos ||
		file.find_first_of('.') != file.find_last_of('.') ||
		*file.begin() != '/' || *file.end() == '.' ||
		file.find("//") != std::string::npos) // check if is path
		return (1);
	this->_errorPage[error] = file;
	if (ss >> file) // check if it has more text
		return (1);
	return (0);
}

int Server::setLocation(const std::string& dir, Location* value)
{
	if ((this->_locations[dir] = value))
		return (0);
	return (1);
}

void Server::initializeMethods()
{
	if (!Server::_methods.empty())
		return;
	_methods["root"] = &CommonDirectives::setRoot;
	_methods["index"] = &CommonDirectives::setIndex;
	_methods["autoindex"] = &CommonDirectives::setAutoindex;
	_methods["upload_store"] = &CommonDirectives::setUploadStore;
	_methods["server_name"] = &Server::setServerNames;
	_methods["listen"] = &Server::setListen;
	_methods["client_max_body_size"] = &Server::setClientMaxBodySize;
	_methods["error_page"] = &Server::setErrorPage;
}

int Server::setDirective(const std::string& directive, const std::string& value)
{
	std::map<std::string, int (Server::*)(const std::string&)>::iterator
		it(_methods.find(directive));
	if (it != _methods.end())
		return ((this->*(it->second))(value));
	return (1);
}

int Server::run()
{
	const int trueFlag = 1;
	std::stringstream port;
	port << this->_listen;

	// to get a non block socket use <SOCK_STREAM | SOCK_NONBLOCK> as second argument
	if ((this->_socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) < 0)
	{
		std::stringstream ss;
		ss << errno;
		MESSAGE("socket(): " + ss.str() + ": " + (std::string)strerror(errno),
				ERROR);
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
			ERROR);
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
				ERROR);
		return (1);
	}

	if (listen(this->_socket, 1) < 0)
	{
		std::stringstream ss;
		ss << errno;
		MESSAGE("listen(): " + ss.str() + ": " + (std::string)strerror(errno),
				ERROR);
		return (1);
	}
	return (0);
}


void Server::getFile(Request &request)
{
	std::fstream		file;
	std::stringstream	ss;
	std::string			length;
	std::string			line;
	std::map<std::string, std::string> header;
	std::vector<std::string>	body;

	file.open(request.getPath().c_str());
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			body.push_back(line);
			ss << file.tellg();
			ss >> length;
		}
	}
	header["HTTP/1.1"] = "200 OK";
	header["Content-Type"] = "text/html";
	header["Content-Length"] = length;
	//TODO: Build response
}

void Server::stop()
{
	if (this->_socket)
		close(this->_socket);
	this->_socket = 0;
}
