/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 12:41:04 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/11/27 18:12:40 by maricard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cluster.hpp"
#include <fstream>

Cluster::Cluster()
{
}

Cluster::Cluster(const Cluster& value) : _serverList(value._serverList)
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
	std::vector<Server*>::iterator it = this->_serverList.begin();
	
	for (; it != this->_serverList.end(); ++it)
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

static int getLocationConfig(Location* location, std::fstream* fstream)
{
	std::string line;

	while (getline(*fstream, line))
	{
		std::stringstream ss(line);
		std::string directive;
		std::string value;

		ss >> directive;
		if (directive.empty() || directive.at(0) == ';' || directive.at(0) == '#')
			continue;
		if (line.find('}') != std::string::npos)
			break;
		if (line.find_first_of(';') == std::string::npos)
		{
			MESSAGE("expected `;' at end of line", ERROR);
			return (1);
		}
		getline(ss, value, ';');
		if ((directive == "cgi_pass" && location->getPath().at(0) == '/'))
		{
			MESSAGE("Unable to configure directive `" + directive +
					"' in specified location block", ERROR);
			return (1);
		}
		if (location->setDirective(directive, value))
		{
			MESSAGE("Unable to configure directive `" + directive + "'", ERROR);
			return (1);
		}
	}
	return (0);
}

static int getServerConfig(std::vector<Server*>* serverList, std::fstream* fstream)
{
	Server server;
	std::string line;

	while (getline(*fstream, line))
	{
		std::stringstream ss;
		std::string directive;
		std::string value;

		if (line.empty() || line.find_first_not_of(" \t") == std::string::npos)
			continue;
		
		ss << line;
		ss >> directive;
		if (directive.empty() || directive.at(0) == ';' || directive.at(0) == '#')
			continue;
		if (directive == "location")
		{
			if (line.find('}') != std::string::npos)
				continue;
			std::string path;
			ss >> path;
			if (path.empty() || (path.at(0) != '.' && path.find('/')) ||
				((path.at(0) != '/' && path.find('.'))) ||
				path.find("//") != std::string::npos) // check if is path
			{
				MESSAGE(path + ": Invalid location path", ERROR);
				return (1);
			}
			if (*path.end() == '/')
				*path.end() = '\0';
			ss >> value;
			if (!value.empty() && value != "{")
			{
				MESSAGE(value + ": Unexpected value", ERROR);
				return (1);
			}
			else if (value.empty())
			{
				while (getline(*fstream, line))
				{
					if (directive.empty() || directive.at(0) == ';' || directive.at(0) == '#')
						continue;
					if (line.find('{') != std::string::npos)
						break;
					if (line.find_first_not_of(" \t") != std::string::npos)
					{
						MESSAGE("Expected `{' on location block declaration", ERROR);
						return (1);
					}
				}
			}
			Location location(path);

			if (getLocationConfig(&location, fstream))
				return (1);
			server.setLocation(path, new Location(location));
			continue;
		}
		if (line.find('}') != std::string::npos)
			break;
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
	serverList->push_back(new Server(server));
	return (0);
}

int Cluster::configure(const std::string& path)
{
	std::fstream fstream;
	if (path.empty() || openFile(path, &fstream))
	{
		MESSAGE("No valid configuration file, using default configuration", WARNING);
		this->_serverList.push_back(new Server());
		return (0);
	}

	std::string line;
	while (getline(fstream, line))
	{
		std::stringstream ss(line);
		std::string block_type;

		ss >> block_type;
		if (block_type.empty() || block_type.at(0) == ';' || block_type.at(0) == '#')
			continue;
		// Check for server block
		if (block_type == "Server")
		{
			std::string bracket;
			ss >> bracket;
			// Check for bracket
			if (bracket.empty())
			{
				while (getline(fstream, line))
				{
					if (!bracket.empty())
						bracket.clear();
					std::stringstream m(line);
					m >> bracket;
					if (bracket.empty())
					{
						MESSAGE("Expected `{' at the end of server block declaration",
								ERROR);
						return (1);
					}
					if (bracket.at(0) == ';' || bracket.at(0) == '#')
						continue;
					if (line.find('}') != std::string::npos)
						break;
					if (!bracket.empty() && bracket.at(0) != '{')
					{
						MESSAGE("Expected `{' on server block declaration",
								ERROR);
						return (1);
					}
					else if (!bracket.empty()) // if found bracket
						break;
				}
			}
			else if (!bracket.empty() && bracket != "{")
			{
				MESSAGE(bracket + ": Unexpected value", ERROR);
				return (1);
			}
			if (line.find('}') == std::string::npos)
			{
				if (getServerConfig(&this->_serverList, &fstream))
					return (1);
			}
		}
		else if (!block_type.empty())
		{
			MESSAGE(block_type + ": Unexpected block", ERROR);
			return (1);
		}
	}
	return (0);
}

static std::string in_addr_t_to_ip(in_addr_t addr)
{
	std::ostringstream oss;

	for (int i = 0; i < 4; ++i)
	{
		in_addr_t octet = (addr >> (i * 8)) & 0xFF;
		oss << octet;

		if (i < 3) {
			oss << ".";
		}
	}
	return oss.str();
}

static void closeConnection(int connection)
{
	MESSAGE("Connection closed gracefully", INFORMATION);
	close(connection);
}

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
	FD_ZERO(&_master_sockets);
	std::vector<Server*>::iterator it = this->_serverList.begin();

	for (; it != this->_serverList.end(); ++it)
	{
		std::stringstream port;
		port << (*it)->getListenPort();
		if ((*it)->run())
		{
			(*it)->stop();
			continue;
		}
		MESSAGE("Listening on " + (*it)->getAddress() + ":" + port.str(), INFORMATION);
		FD_SET((*it)->getSocket(), &_master_sockets);
	}
	if (!isAnyServerRunning(_master_sockets))
	{
		MESSAGE("No servers were created", ERROR);
		return;
	}
	
	while (true)
	{
		_read_sockets = _master_sockets;
		_write_sockets = _master_sockets;
		int connection = -1;
		std::string response;

		int selctResult = select(FD_SETSIZE, &_read_sockets, &_write_sockets, NULL, NULL);
		if (selctResult < 0)
		{
			std::stringstream ss;
			ss << errno;
			MESSAGE( "select(): " + ss.str() + ": " + (std::string)strerror(errno), ERROR);
			continue;
		}
		else if (selctResult == 0)
			continue;

		acceptNewConnections(connection);
		
		std::map<Server*, int>::iterator it = _connections.begin();
		for (; it != _connections.end(); it++)
		{
			readRequest(it->first, it->second, response);
			sendResponse(it->second, response);
		}
	}
}

void	Cluster::acceptNewConnections(int connection)
{
	std::vector<Server*>::iterator it = this->_serverList.begin();
	struct sockaddr_in clientAddress = {};
	socklen_t clientAddressLength = sizeof(clientAddress);
	
	for (; it != this->_serverList.end(); ++it)
	{
		if (!(*it)->getSocket())
			continue;
		
		if (FD_ISSET((*it)->getSocket(), &this->_read_sockets))
		{
			if ((connection = accept((*it)->getSocket(), (struct sockaddr*)&clientAddress,
				(socklen_t*)&clientAddressLength)) < 0)
			{
				std::stringstream ss;
				ss << errno;
				MESSAGE("accept(): " + ss.str() + ": " + (std::string)strerror(errno), ERROR);
				continue;
			}
			
			FD_SET(connection, &this->_read_sockets);
			_connections[(*it)] = connection;
		}
	}
}

void	Cluster::readRequest(Server* server, int connection, std::string& response)
{
	struct sockaddr_in clientAddress = {};
	int error = 0;
	
	if (FD_ISSET(connection, &this->_read_sockets))
	{
		Request request(server, connection);
		int64_t bytesRead;
		int64_t bytesLeftToRead = 4096;
		char 	header_buffer[bytesLeftToRead];
		std::stringstream	port;
		
		port << server->getListenPort();
		MESSAGE("Connected " + in_addr_t_to_ip(clientAddress.sin_addr.s_addr) + " to " + port.str(), INFORMATION);
		
		for (size_t i = 0; i < sizeof(header_buffer); ++i)
			header_buffer[i] = '\0';

		if ((bytesRead = recv(connection, header_buffer, bytesLeftToRead, 0)) > 0)
		{
			error = request.parseRequest(header_buffer, bytesRead);
			
			//if (!error && bytesLeftToRead)
			//	error = 400;

			request.displayVars();
			
			std::stringstream host(request.getHeader()["Host"]);
			std::string serverName;
			std::vector<Server*>::iterator hostIt = this->_serverList.begin();

			std::getline(host, serverName, ':');
			for (; hostIt != this->_serverList.end(); ++hostIt)
			{
				if ((*hostIt)->isServerName(serverName))
				{
					server = *hostIt;
					return ;
				}
			}
			Response::setResponseServer(server);
			
			int selectedOptions = request.isValidRequest((*server), error);

			if (!error)
				response = checkRequestedOption(selectedOptions, request, server);
		}
		else
		{
			close(connection);
			return ;
		}

		if (error)
			response = Response::buildErrorResponse(error);

		MESSAGE("Request processed successfully", INFORMATION);
		FD_CLR(connection, &this->_read_sockets);
		FD_SET(connection, &this->_write_sockets);
	}
}

std::string	Cluster::checkRequestedOption(int selectedOptions, Request& request, Server* server)
{
	if (selectedOptions & REDIR)
		return server->redirect(request);
	else if (selectedOptions & DIR_LIST)
		return server->directoryListing(request);
	else if (selectedOptions & CGI)
	{
		Cgi cgi(request);
		return cgi.runCGI();
	}
	else if (selectedOptions & DELETE)
		return server->deleteFile(request);
	else if (selectedOptions & GET)
		return server->getFile(request);
	return Response::buildErrorResponse(500);
}

void	Cluster::sendResponse(int connection, std::string& response)
{
	if (FD_ISSET(connection, &this->_write_sockets))
	{
		if (send(connection, response.c_str(), response.size(), 0) < 0)
			MESSAGE("Sending response to socket", ERROR);

		std::string status_code = response.substr(9, 3);
		std::string status_message = response.substr(12, response.find(CRLF) - 12);

		std::stringstream ss(status_code);
		int status;
		ss >> status;

		if (status < 400) {
			MESSAGE(status_code + status_message, OK); }
		else {
			MESSAGE(status_code + status_message, ERROR); }

		closeConnection(connection);
		FD_CLR(connection, &this->_write_sockets);
	}
}