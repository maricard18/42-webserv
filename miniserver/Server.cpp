/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 12:51:47 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/10/13 13:45:00 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
	: listen_port(0), client_max_body_size(1), autoindex(false)
{
}

Server::Server(const Server& value)
	: server_names(value.server_names),
	  listen_ip(value.listen_ip),
	  listen_port(value.listen_port),
	  client_max_body_size(value.client_max_body_size),
	  locations(value.locations),
	  autoindex(false)
{
}

Server& Server::operator=(const Server& value)
{
	if (*this == value)
		return (*this);
	// TODO
	return (*this);
}

Server::~Server()
{
}

std::vector<std::string> Server::getServerNames() const
{
	return (this->server_names);
}

std::string Server::getListenIP() const
{
	return (this->listen_ip);
}

int Server::getListenPort() const
{
	return (this->listen_port);
}

int Server::getClientMaxBodySize() const
{
	return (this->client_max_body_size);
}

std::map<std::string, Location> Server::getLocations() const
{
	return (this->locations);
}
