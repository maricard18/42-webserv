/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 15:58:21 by maricard          #+#    #+#             */
/*   Updated: 2023/10/28 16:36:37 by bsilva-c         ###   ########.fr       */
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
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <cstdio>

#define READ 0
#define WRITE 1

class Server;
class Request
{
	protected:
		std::string _method;
		std::string _path;
		std::string _query;
		std::string _protocol;
		std::map<std::string, std::string> _header;
		std::vector<char> _body;
		char*	_argv[3];
		char*	_envp[17];
		char*	_buffer;
		u_int32_t 	_bodyLength;
		u_int32_t 	_maxBodySize;
		std::string _uploadStore;

	public:
		Request();
		Request(char* buffer, Server* server);
		Request(const Request& copy);
		~Request();
		Request& operator=(const Request& other);

		std::string getMethod() const;
		std::string getPath() const;
		std::string getQuery() const;
		std::string getProtocol() const;

		int		parseRequest(char* buffer, int bytesRead);
		int		handleRequest(char* buffer, int bytesRead);
		void	handleBody(char* buffer, int bytesRead);
		int		isValidRequest(Server* server);
		bool	hasCGI();
		void	runCGI();

		void	setArgv();
		void	setEnvp();
		bool	checkErrors();
		void	deleteMemory();
		char*	myStrdup(const char* source);
		void	displayVars();
};


//! add _query
//! create and load it on .cpp
//! create ENV VAR
