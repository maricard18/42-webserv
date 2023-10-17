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
	: CommonDirectives("/"), _uploadStore("/"), _cgiPass("/")
{
	this->_allowMethods.push_back("GET");
	this->_allowMethods.push_back("POST");
	this->_allowMethods.push_back("DELETE");
}

Location::Location(const std::string& path)
	: CommonDirectives(path), _uploadStore(path), _cgiPass(path)
{
	this->_allowMethods.push_back("GET");
	this->_allowMethods.push_back("POST");
	this->_allowMethods.push_back("DELETE");
}

Location::Location(const Location& value)
	: CommonDirectives(value._root, value._index, value._autoindex),
	  _allowMethods(value._allowMethods),
	  _uploadStore(value._uploadStore),
	  _cgiPass(value._cgiPass)
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
	return (this->_uploadStore);
}

std::string Location::getCgiPass() const
{
	return (this->_cgiPass);
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
	this->_allowMethods = allowMethods;
	return (0);
}

int Location::setUploadStore(const std::string& value)
{
	std::stringstream ss(value);
	std::string dir;

	ss >> dir;
	if (dir.at(0) != '/') // check if is path
		return (1);
	this->_uploadStore = dir;
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
	this->_cgiPass = dir;
	if (ss >> dir) // check if it has more text
		return (1);
	return (0);
}

bool Location::isMethodAllowed(const std::string& method)
{
	for (std::vector<std::string>::iterator it = this->_allowMethods.begin();
		 it != this->_allowMethods.end(); ++it)
		if (*it == method)
			return (true);
	return (false);
}
