/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 12:41:04 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/10/27 17:44:59 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cluster.hpp"

Cluster::Cluster()
{
}

Cluster::Cluster(const Cluster& value)
	: _serverList(value._serverList)
{
	*this = value;
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
	for (std::vector<Server*>::iterator it = this->_serverList.begin();
		 it != this->_serverList.end(); ++it)
	{
		delete *it;
		*it = 0;
	}
}

int Cluster::configure(const std::string& path)
{
	if (path.empty())
	{
		this->_serverList.push_back(new Server());
		return (0);
	}
	return (0);
}

// Check if there are any servers running
static bool isAnyServerRunning(fd_set& set)
{
	for (int i = 0; i < FD_SETSIZE; ++i)
	{
		if (FD_ISSET(i, &set))
			return (true);
	}
	return (false);
}

void Cluster::run()
{
	fd_set current_sockets, ready_sockets;
	FD_ZERO(&current_sockets);
	// Bind Server to Socket.
	for (std::vector<Server*>::iterator it = this->_serverList.begin();
		 it != this->_serverList.end(); ++it)
	{
		std::stringstream port;
		port << (*it)->getListenPort();
		MESSAGE("Booting " + (*it)->getAddress() + ":" + port.str(),
				INFORMATION);
		if ((*it)->run())
		{
			(*it)->stop();
			continue;
		}
		MESSAGE("Listening on " + (*it)->getAddress() + ":" + port.str(),
				INFORMATION);
		FD_SET((*it)->getSocket(), &current_sockets);
	}
	if (!isAnyServerRunning(current_sockets))
	{
		MESSAGE("No servers were created", ERROR);
		return;
	}
	while (true)
	{
		ready_sockets = current_sockets;
		//! check read and write at the same time
		if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0)
		{
			std::stringstream ss;
			ss << errno;
			MESSAGE(
				"select(): " + ss.str() + ": " +
				(std::string)strerror(errno),
				ERROR);
			continue;
		}

		int connection;
		for (std::vector<Server*>::iterator it = this->_serverList.begin();
			 it != this->_serverList.end(); ++it)
		{
			if (!(*it)->getSocket())
				continue;
			if (FD_ISSET((*it)->getSocket(), &ready_sockets))
			{
				u_int32_t address_length = sizeof((*it)->getServerAddress());
				if ((connection = accept((*it)->getSocket(),
										 (struct sockaddr*)&(*it)->getServerAddress(),
										 (socklen_t*)&address_length)) < 0)
				{
					std::stringstream ss;
					ss << errno;
					MESSAGE("accept(): " + ss.str() + ": " +
							(std::string)strerror(errno), ERROR);
					continue;
				}
				MESSAGE("Connected with a client", INFORMATION);

				int64_t bytesRead;
				int64_t bytesToRead = 4096;
				char header_buffer[4096];
				
				MESSAGE("READ STARTED", WARNING);


				if ((bytesRead = read(connection, header_buffer, 4096)) >= 0)
				{
					Request request(header_buffer);

					if (bytesRead < 4096)
						request.handleRequest(header_buffer, bytesRead);	
					else
					{
						bytesToRead = request.handleRequest(header_buffer, bytesRead);
						char body_buffer[bytesToRead]; 
						bytesRead = read(connection, body_buffer, bytesToRead);
						request.handleBody(body_buffer, bytesRead);
					}

					if(request.hasCGI() == true)
					{
						MESSAGE("CGI running", WARNING);
						request.runCGI();
						MESSAGE("CGI finished", WARNING);
					}

					//request.displayVars();
				}
				else
				{
					MESSAGE("READ ERROR", ERROR)
					return ;
				}

				MESSAGE("READ FINISHED", WARNING);

				std::ifstream file("post_response.txt");
				std::stringstream stream;
				
				stream << file.rdbuf();
				std::string response = stream.str();
				send(connection, response.c_str(), response.size(), 0);
				
				MESSAGE("Closed connection", INFORMATION);
				close(connection);
			}
		}
	}
}
