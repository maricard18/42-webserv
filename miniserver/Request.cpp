/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 17:14:44 by maricard          #+#    #+#             */
/*   Updated: 2023/10/18 17:52:42 by maricard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request()
{	
}

Request::Request(std::string request)
{
	parseRequest(request);
}

Request::Request(const Request& copy)
{
	*this = copy;
}

Request::~Request()
{

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

std::string Request::getMethod()
{
	return _method;
}

std::string Request::getPath()
{
	return _path;
}

std::string Request::getProtocol()
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
			std::string second = line.substr(pos + 2);
			_header[first] = second;
    	}
    }

	while (std::getline(ss, line))
	{
    	_body.push_back(line);
    }

	displayVars();
	
	//std::cout << F_WHITE "body: " RESET + ss.str() << std::endl;
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