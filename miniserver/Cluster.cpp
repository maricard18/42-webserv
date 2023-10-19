/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 12:41:04 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/10/19 18:40:33 by maricard         ###   ########.fr       */
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

void Cluster::configure(const std::string& path)
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
	if (this->_serverList.empty())
	{
		MESSAGE("No servers were created", CRITICAL);
		return;
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
<<<<<<< HEAD
		std::cout << std::endl << F_GREEN "server and client connected successfully!" RESET
				  << std::endl;
=======
		MESSAGE("Connected with a client", INFORMATION);
>>>>>>> base

		char buffer[100000];
		int64_t bytesRead = read(connection, buffer, 100000);
		if (bytesRead == -1)
		{
			close(connection);
			continue;
		}
<<<<<<< HEAD
=======
		std::cout << buffer << std::endl;
>>>>>>> base

		Request request(buffer);

		std::string response = HTML;
		send(connection, response.c_str(), response.size(), 0);
<<<<<<< HEAD
		std::cout << F_RED "Closed connection" RESET << std::endl;
=======
		MESSAGE("Closed connection", INFORMATION);
>>>>>>> base
		close(connection);
	}
}
