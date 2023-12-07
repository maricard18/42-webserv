/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 15:58:21 by maricard          #+#    #+#             */
/*   Updated: 2023/12/01 18:42:07 by maricard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <stdio.h>
#include <fstream>
#include "macros.hpp"
#include "Server.hpp"
#include "Response.hpp"
#include "Cluster.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <cstdio>
#include "utils.hpp"

class Cluster;
class Server;

class Request
{
	private:
		std::string _method;
		std::string _path;
		std::string _executable;
		std::string _query;
		std::string _protocol;
		std::map<std::string, std::string> _header;
		std::vector<char> _body;
		u_int32_t 	_bodyLength;
		u_int32_t 	_maxBodySize;
		std::string _uploadStore;
		int 		_connection;
		Server*		_server;
		
		Request();
		Request(const Request& copy);
		Request& operator=(const Request& other);
	
	public:
		Request(int connection);
		~Request();

		std::string getMethod() const;
		std::string getPath() const;
		std::string getQuery() const;
		std::string getProtocol() const;
		std::map<std::string, std::string>	getHeader() const;
		std::vector<char>	getBody() const;
		std::string getUploadStore() const;
		std::string getExtension();
		std::string getExecutable() const;
		std::string getHeaderField(const std::string& field);
		Server* getServer() const;

		void	setServer(Server* server);
		int		parseRequest(Cluster& cluster, char* buffer, int64_t bytesAlreadyRead);
		int		parseBody(char* chunk, int64_t bytesToRead);
		int		parseChunkedRequest(char* buffer, int64_t bytesToRead);
		int		checkErrors();
		void	displayVars();
		int		isValidRequest(Server& server, int& error);
};
