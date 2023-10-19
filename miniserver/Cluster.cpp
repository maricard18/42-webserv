/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 12:41:04 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/10/20 20:51:51 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cluster.hpp"
#include <fstream>

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
	for (std::vector<Server*>::iterator it = this->_serverList.begin();
		 it != this->_serverList.end(); ++it)
	{
		delete *it;
		*it = 0;
	}
}

static int openFile(const std::string& file_path, std::fstream* fstream)
{
	if (fstream->is_open())
		fstream->close();
	fstream->open(file_path.c_str(), std::ios::in);
	if (!fstream->is_open())
	{
		MESSAGE(file_path + ": " + strerror(errno), ERROR);
		fstream->clear();
		return (1);
	}
	return (0);
}

/* TODO
static void getLocationConfig(std::vector<Server>* serverList,
							  std::fstream* fstream)
{

}*/

static int getServerConfig(std::vector<Server>* serverList,
						   std::fstream* fstream)
{
	Server server;
	std::string line;

	while (getline(*fstream, line) && !line.empty())
	{
		std::stringstream ss(line);
		std::string directive;
		std::string value;

		ss >> directive;
		if (line.find_first_of(';') == std::string::npos)
		{
			MESSAGE("expected `;' at end of line", ERROR);
			return (1);
		}
		getline(ss, value, ';');
		if (server.setDirective(directive, value))
		{
			MESSAGE("Unable to configure directive `" + directive + "'", ERROR);
			return (1);
		}
	}
	serverList->push_back(server);
	return (0);
}

void Cluster::configure(const std::string& file_path)
{
	std::fstream fstream;
	if (file_path.empty() || openFile(file_path, &fstream))
	{
	runDefault:
		MESSAGE("No valid configuration file, using default configuration",
				WARNING);
		this->_serverList.push_back(new Server());
		return;
	}

	std::string line;
	while (line.find("Server") == std::string::npos)
	{
		if (!line.empty())
		{
			MESSAGE("Invalid configuration file", ERROR);
			goto runDefault;
		}
		getline(fstream, line);
	}
	if (getServerConfig(&this->_serverList, &fstream))
		goto runDefault;
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
		MESSAGE("Setting up " + (*it)->getAddress() + ":" + port.str(),
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
		if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0)
		{
			std::stringstream ss;
			ss << errno;
			MESSAGE(
				"select(): " + ss.str() + ": " +
				(std::string)strerror(errno),
				ERROR);
			return;
		}

		int connection = -1;
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
					return;
				}
			}
		}
		if (connection == -1)
			continue;
		MESSAGE("Connected with a client", INFORMATION);

		char buffer[8192];
		int64_t bytesRead = read(connection, buffer, 8192);
		if (bytesRead == -1)
		{
			close(connection);
			continue;
		}
		std::cout << buffer << std::endl;

		std::string response =
			"HTTP/1.1 200 OK\r\n\r\nHello how are you?\n\nI am the server\n";
		send(connection, response.c_str(), response.size(), 0);
		MESSAGE("Closed connection", INFORMATION);
		close(connection);
	}
}
