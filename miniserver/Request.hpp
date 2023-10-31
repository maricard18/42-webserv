/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 15:58:21 by maricard          #+#    #+#             */
/*   Updated: 2023/10/21 14:22:11 by maricard         ###   ########.fr       */
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

class Request
{
	private:
		std::string _method;
		std::string _path;
		std::string _protocol;
		std::map<std::string, std::string> _header;
		std::vector<char> _body;
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
		std::string getProtocol() const;

		int		handleRequest(char* buffer, int bytesRead);
		void	handleBody(char* buffer, int bytesRead);
		int		parseRequest(char* buffer, int bytesRead);
		bool	hasCGI();
		void	runCGI();
		void	displayVars();
};
