/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 12:51:47 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/10/15 22:07:36 by bsilva-c         ###   ########.fr       */
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
	: CommonDirectives("/var/www/html", false),
	  address(ip_to_in_addr_t("0.0.0.0")),
	  listen(0),
	  client_max_body_size(1000)
{
}

Server::Server(const std::string& root,
			   const std::vector<std::string>& index,
			   u_int16_t listen)
	: CommonDirectives(root, index, false),
	  address(ip_to_in_addr_t("0.0.0.0")),
	  listen(listen),
	  client_max_body_size(1000)
{
}

Server::Server(const Server& value)
	: CommonDirectives(value.root, value.index, value.autoindex),
	  server_names(value.server_names),
	  address(value.address),
	  listen(value.listen),
	  client_max_body_size(value.client_max_body_size),
	  error_page(value.error_page),
	  locations(value.locations)
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
		std::map<std::string, Location*>::iterator it = this->locations.begin();
		it != this->locations.end(); ++it)
		delete it->second;
}

std::vector<std::string> Server::getServerNames() const
{
	return (this->server_names);
}

in_addr_t Server::getAddress() const
{
	return (this->address);
}

u_int16_t Server::getListenPort() const
{
	return (this->listen);
}

u_int32_t Server::getClientMaxBodySize() const
{
	return (this->client_max_body_size);
}

std::string Server::getErrorPage(int error_code)
{
	return (this->error_page[error_code]);
}

Location& Server::getLocation(const std::string& location)
{
	return (*this->locations[location]);
}

int Server::setServerNames(const std::string& value)
{
	std::string buf;
	std::stringstream ss(value);

	while (ss >> buf)
	{
		if (*buf.begin() != '.' && *buf.end() != '.')
			this->server_names.push_back(buf);
		else
			return (1);
	}
	return (0);
}

int Server::setAddress(const std::string& value)
{
	if (!(this->address = ip_to_in_addr_t(value)))
		return (1);
	return (0);
}

int Server::setClientMaxBodySize(const std::string& value)
{
	if (value.find_first_not_of("0123456789kKmM") != std::string::npos)
		return (1);
	std::string unit;
	std::stringstream ss(value);

	ss >> this->client_max_body_size;
	if (this->client_max_body_size == 0)
		return (1);
	ss >> unit;
	if (unit == "k" || unit == "K")
		this->client_max_body_size *= 1000;
	else if (unit == "m" || unit == "M")
		this->client_max_body_size *= 1000000;
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
	this->error_page[error] = file;
	if (ss >> file) // check if it has more text
		return (1);
	return (0);
}

int Server::setLocation(const std::string& dir, const Location& value)
{
	this->locations[dir] = new Location(value);
}
