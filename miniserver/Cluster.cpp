/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 12:41:04 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/10/21 14:25:54 by maricard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cluster.hpp"

Cluster::Cluster()
{
}

Cluster::Cluster(const Cluster& value)
	: _serverList(value._serverList)
{
}

Cluster& Cluster::operator=(const Cluster& value)
{
	if (this == &value)
		return (*this);

	Cluster tmp(value);

	std::swap(*this, tmp);
	return (*this);
}

Cluster::~Cluster()
{
}

void Cluster::setup(const std::string& path)
{
	if (path.empty())
	{
		this->_serverList.push_back(Server());
		return;
	}
}

void Cluster::run()
{
	fd_set current_sockets, ready_sockets;
	FD_ZERO(&current_sockets);
	// Bind Server to Socket.
	for (std::vector<Server>::iterator it = this->_serverList.begin();
		 it != this->_serverList.end(); ++it)
	{
		std::stringstream port;
		port << it->getListenPort();
		MESSAGE("Setting up " + it->getAddress() + ":" + port.str(),
				INFORMATION);
		if (it->run())
		{
			it->stop();
			continue;
		}
		MESSAGE("Listening on " + it->getAddress() + ":" + port.str(),
				INFORMATION);
		FD_SET(it->getSocket(), &current_sockets);
	}
	while (true)
	{
		ready_sockets = current_sockets;
		if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0)
		{
			std::stringstream ss;
			ss << errno;
			MESSAGE(
				"select(): " + ss.str() + ": " + (std::string)strerror(errno),
				CRITICAL);
			return;
		}

		int connection = -1;
		for (std::vector<Server>::iterator it = this->_serverList.begin();
			 it != this->_serverList.end(); ++it)
		{
			if (!it->getSocket())
				continue;
			if (FD_ISSET(it->getSocket(), &ready_sockets))
			{
				u_int32_t address_length = sizeof(it->getServerAddress());
				if ((connection = accept(it->getSocket(),
										 (struct sockaddr*)&it->getServerAddress(),
										 (socklen_t*)&address_length)) < 0)
				{
					std::stringstream ss;
					ss << errno;
					MESSAGE("accept(): " + ss.str() + ": " +
							(std::string)strerror(errno), CRITICAL);
					return;
				}
			}
		}
		if (connection == -1)
			continue;
		MESSAGE("Connected with a client", INFORMATION);

		char buffer[100000];
		int64_t bytesRead = read(connection, buffer, 100000);
		if (bytesRead == -1)
		{
			close(connection);
			continue;
		}
		std::cout << buffer << std::endl;

		Request request(buffer);

		std::string response = HTML;
		send(connection, response.c_str(), response.size(), 0);
		MESSAGE("Closed connection", INFORMATION);
		close(connection);
	}
}
