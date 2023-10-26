/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 17:14:44 by maricard          #+#    #+#             */
/*   Updated: 2023/10/21 14:22:35 by maricard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request()
{	
}

Request::Request(std::string request)
{
	parseRequest(request);
	
	// if(hasCGI() == true)
	// 	runCGI();
}

Request::Request(const Request& copy)
{
	*this = copy;
}

Request::~Request()
{
	_header.clear();
	_body.clear();
}

Request& Request::operator=(const Request& other)
{
	if (this == &other)
		return *this;

	_method = other._method;
	_path = other._path;
	_protocol = other._protocol;
	return *this;
}

std::string Request::getMethod() const
{
	return _method;
}

std::string Request::getPath() const
{
	return _path;
}

std::string Request::getProtocol() const
{
	return _protocol;
}

void	Request::parseRequest(std::string request)
{
	std::stringstream ss(request);
	std::string line;

	ss >> _method >> _path >> _protocol;

	std::getline(ss, line);
	while (std::getline(ss, line) && line != "\r")
	{
    	size_t pos = line.find(':');
    
		if (pos != std::string::npos)
		{
			std::string first = line.substr(0, pos);
			std::string second = line.substr(pos + 2, line.length());
			_header[first] = second;
    	}
    }

	if (_method == "POST" && _header["Content-Length"].empty())
	{
		// error 411 Length Required
		MESSAGE("POST request without Content-Length", ERROR);
		return;
	}
	else if (_method == "POST" && (_header["Content-Type"].empty() ||
		_header["Content-Type"].find("multipart/form-data") == std::string::npos))
	{
		// error 415 Unsupported Media Type
		MESSAGE("POST request without Content-Type", ERROR);
		return;
	}
	
	while (std::getline(ss, line))
	{
		_body.push_back(line);
	}

	displayVars();
}

void	Request::displayVars()
{
	std::cout << std::endl;
	std::cout << F_YELLOW "Request values" RESET << std::endl;
	std::cout << F_YELLOW "Method: " RESET + _method << std::endl;
	std::cout << F_YELLOW "Path: " RESET + _path << std::endl;
	std::cout << F_YELLOW "Protocol: " RESET + _protocol << std::endl;

	if (_header.size() > 0)
		std::cout << F_YELLOW "header:" RESET << std::endl;
	
	std::map<std::string, std::string>::iterator it = _header.begin();
	for (; it != _header.end(); it++)
		std::cout << it->first + ": " << it->second << std::endl;
		
	std::cout << std::endl;

	if (_body.size() > 0)
		std::cout << F_YELLOW "body:" RESET << std::endl;

	for (unsigned i = 0; i < _body.size(); i++)
		std::cout << _body[i] << std::endl;
}