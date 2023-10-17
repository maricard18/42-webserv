/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 17:14:44 by maricard          #+#    #+#             */
/*   Updated: 2023/10/17 20:11:12 by maricard         ###   ########.fr       */
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

void	Request::parseRequest(std::string request)
{
	std::stringstream ss(request);
	std::string line;

	ss >> _method >> _path >> _protocol;

	while (std::getline(ss, line) && !line.empty())
	{
    	size_t pos = line.find(':');
    
		if (pos != std::string::npos)
		{
			std::string first = line.substr(0, pos);
			std::string second = line.substr(pos + 2);
			_body[first] = second;
    	}
    }

	displayVars();
}

void	Request::displayVars()
{
	std::cout << std::endl;
	std::cout << "Request values" << std::endl;
	std::cout << "Method: " + _method << std::endl;
	std::cout << "Path: " + _path << std::endl;
	std::cout << "Protocol: " + _protocol << std::endl;

	std::map<std::string, std::string>::iterator it = _body.begin();
	for (; it != _body.end(); it++)
		std::cout << it->first + ": " << it->second << std::endl;
}