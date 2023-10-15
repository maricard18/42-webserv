/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 12:51:47 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/10/15 17:03:01 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// TODO better way to initialize w/ CommonDirectives
Server::Server()
	: listen(0), client_max_body_size(1)
{
}

Server::Server(const std::vector<std::string>& server_names,
			   int listen,
			   int client_max_body_size,
			   const std::map<int, std::string>& error_page,
			   const std::map<std::string, Location>& locations)
	: server_names(server_names),
	  listen(listen),
	  client_max_body_size(client_max_body_size),
	  error_page(error_page),
	  locations(locations)
{
}

Server::Server(const Server& value)
	: CommonDirectives(value),
	  server_names(value.server_names),
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
}

std::vector<std::string> Server::getServerNames() const
{
	return (this->server_names);
}

int Server::getListenPort() const
{
	return (this->listen);
}

int Server::getClientMaxBodySize() const
{
	return (this->client_max_body_size);
}

std::map<int, std::string> Server::getErrorPages() const
{
	return (this->error_page);
}

std::map<std::string, Location> Server::getLocations() const
{
	return (this->locations);
}
