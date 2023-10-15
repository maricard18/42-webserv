/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 13:01:17 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/10/15 21:42:23 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location()
	: CommonDirectives("/", false), upload_store("/"), cgi_pass("/")
{
	this->allow_methods.push_back("GET");
	this->allow_methods.push_back("POST");
	this->allow_methods.push_back("DELETE");
}

Location::Location(const std::string& path)
	: CommonDirectives(path), upload_store(path), cgi_pass(path)
{
	this->allow_methods.push_back("GET");
	this->allow_methods.push_back("POST");
	this->allow_methods.push_back("DELETE");
}

Location::Location(const Location& value)
	: CommonDirectives(value),
	  allow_methods(value.allow_methods),
	  upload_store(value.upload_store),
	  cgi_pass(value.cgi_pass)
{
}

Location& Location::operator=(const Location& value)
{
	if (this == &value)
		return (*this);

	Location tmp(value);

	std::swap(*this, tmp);
	return (*this);
}

Location::~Location()
{
}

std::string Location::getUploadStore() const
{
	return (this->upload_store);
}

std::string Location::getCgiPass() const
{
	return (this->cgi_pass);
}

int Location::setAllowMethods(const std::string& value)
{
	std::vector<std::string> allowMethods;
	std::stringstream ss(value);
	std::string method;

	while (ss >> method)
	{
		if (method == "GET" || method == "POST" || method == "DELETE")
		{
			for (std::vector<std::string>::iterator it = allowMethods.begin();
				 it != allowMethods.end(); ++it)
				if (*it == method)
					return (1);
			allowMethods.push_back(method);
		}
		else
			return (1);
	}
	this->allow_methods = allowMethods;
	return (0);
}

int Location::setUploadStore(const std::string& value)
{
	std::stringstream ss(value);
	std::string dir;

	ss >> dir;
	if (dir.at(0) != '/') // check if is path
		return (1);
	this->upload_store = dir;
	if (ss >> dir) // check if it has more text
		return (1);
	return (0);
}

int Location::setCgiPass(const std::string& value)
{
	std::stringstream ss(value);
	std::string dir;

	ss >> dir;
	if (dir.at(0) != '/') // check if is path
		return (1);
	this->cgi_pass = dir;
	if (ss >> dir) // check if it has more text
		return (1);
	return (0);
}

bool Location::isMethodAllowed(const std::string& method)
{
	for (std::vector<std::string>::iterator it = this->allow_methods.begin();
		 it != this->allow_methods.end(); ++it)
		if (*it == method)
			return (true);
	return (false);
}
