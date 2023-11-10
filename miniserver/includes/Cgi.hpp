/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 12:41:18 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/11/10 20:47:30 by maricard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Request.hpp"
#include <unistd.h>
#include <fstream>
#include <iostream>

#define READ 0
#define WRITE 1

class Cgi
{
	private:
		std::string _method;
		std::string _path;
		std::string _executable;
		std::string _query;
		std::string	_uploadStore;
		char*		_argv[3];
		char*		_envp[17];
		std::map<std::string, std::string> 	_header;
		std::vector<char> 	_body;

	public:
		Cgi();
		Cgi(Request& request);
		Cgi(const Cgi&);
		Cgi& operator=(const Cgi&);
		~Cgi();

		std::string	runCGI();
		std::string readDataFromCgi(int fd);
		int 		sendDataToCgi(std::string filename, FILE*& file);
		std::string	setArgv();
		void		setEnvp();
		char*		myStrdup(const char* source);
};
