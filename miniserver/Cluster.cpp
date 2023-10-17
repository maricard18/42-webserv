/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 12:41:04 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/10/17 20:03:45 by bsilva-c         ###   ########.fr       */
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
	for (std::vector<Server>::iterator it = this->_serverList.begin();
		 it != this->_serverList.end(); ++it)
	{
		it->run();
		FD_SET(it->getSocket(), &current_sockets);
	}
	while (true)
	{
		ready_sockets = current_sockets;

		if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0)
		{
			std::cerr << "Error on select." << std::endl
					  << "errno: " << errno << std::endl;
			return;
		}

		int connection = -1;
		for (std::vector<Server>::iterator it = this->_serverList.begin();
			 it != this->_serverList.end(); ++it)
		{
			if (FD_ISSET(it->getSocket(), &ready_sockets))
			{
				u_int32_t address_length = sizeof(it->getServerAddress());
				if ((connection = accept(it->getSocket(),
										 (struct sockaddr*)&it->getServerAddress(),
										 (socklen_t*)&address_length)) < 0)
				{
					std::cerr << "Error on accept." << std::endl
							  << "errno: " << errno << std::endl;
					return;
				}
			}
		}
		if (connection == -1)
			continue;
		std::cout << std::endl << "server and client connected successfully!"
				  << std::endl;

		char buffer[8192];
		int64_t bytesRead = read(connection, buffer, 8192);
		if (bytesRead == -1)
		{
			close(connection);
			continue;
		}
		buffer[bytesRead] = 0;
		std::cout << buffer << std::endl;

		std::string response =
			"HTTP/1.1 200 OK\r\n\r\nHello how are you?\n\nI am the server\n";
		send(connection, response.c_str(), response.size(), 0);
		std::cout << "Closed connection" << std::endl;
		close(connection);
	}
//	close(_socket[0]);
//	close(_socket[1]);
}
