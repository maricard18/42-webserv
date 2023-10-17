/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 12:41:04 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/10/17 21:37:50 by maricard         ###   ########.fr       */
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
		std::cout << std::endl << F_GREEN "server and client connected successfully!" RESET
				  << std::endl;

		//char buffer[8192];
		//int bytesRead = -1;
		//std::ifstream file("request.txt", std::ios::binary);
		
		//file.read(buffer, sizeof(buffer));
        //bytesRead = file.gcount();
        //file.close();
		//if (bytesRead == -1)
		//{
		//	close(connection);
		//	continue;
		//}

		char buffer[100000];
		int64_t bytesRead = read(connection, buffer, 100000);
		if (bytesRead == -1)
		{
			close(connection);
			continue;
		}
		
		//std::cout << buffer << std::endl;
		Request request(buffer);

		std::string response =
			"HTTP/1.1 200 OK\r\n\r\n<html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>File Upload Example</title></head><body><h1>File Upload Example</h1><form action=\"/upload\" method=\"POST\" enctype=\"multipart/form-data\"><input type=\"file\" name=\"file\" required><button type=\"submit\">Upload File</button></form></body></html>";
		send(connection, response.c_str(), response.size(), 0);
		std::cout << F_RED "Closed connection" RESET << std::endl;
		close(connection);
	}
//	close(_socket[0]);
//	close(_socket[1]);
}
