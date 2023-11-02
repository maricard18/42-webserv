/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 15:58:21 by maricard          #+#    #+#             */
/*   Updated: 2023/10/26 15:37:30 by maricard         ###   ########.fr       */
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
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <cstdio>

#define READ 0
#define WRITE 1

class Request
{
	protected:
		std::string _method;
		std::string _path;
		std::string _query;
		std::string _protocol;
		std::map<std::string, std::string> _header;
		std::vector<char> _body;
		char *_argv[3];
		char *_envp[17];
		char* _buffer;
		int _bodyLength;

	public:
		Request();
		Request(char* buffer);
		Request(const Request& copy);
		~Request();
		Request& operator=(const Request& other);

		std::string getMethod() const;
		std::string getPath() const;
		std::string getQuery() const;
		std::string getProtocol() const;

		void	setArgv();
		void	setEnvp();

		int		parseRequest(char* buffer, int bytesRead);
		int		handleRequest(char* buffer, int bytesRead);
		void	handleBody(char* buffer, int bytesRead);
		bool	hasCGI();
		void	runCGI();
		void	displayVars();
};


//! add _query
//! create and load it on .cpp
//! create ENV VAR