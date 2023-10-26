/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 13:01:17 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/10/20 15:33:49 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

std::map<std::string, int (Location::*)(const std::string&)> Location::_methods;

Location::Location()
	: CommonDirectives("/"), _cgiPass("/")
{
	this->_allowMethods.push_back("GET");
	this->_allowMethods.push_back("POST");
	this->_allowMethods.push_back("DELETE");
}

Location::Location(const std::string& path)
	: CommonDirectives(path), _cgiPass(path)
{
	Location::initializeMethods();
	this->_allowMethods.push_back("GET");
	this->_allowMethods.push_back("POST");
	this->_allowMethods.push_back("DELETE");
}

Location::Location(const Location& value)
	: CommonDirectives(value._root,
					   value._index,
					   value._uploadStore,
					   value._autoindex),
	  _allowMethods(value._allowMethods),
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

std::string Location::getCgiPass() const
{
	return (this->_cgiPass);
}

std::pair<int, std::string> Location::getRedirect() const
{
	return (this->_redirect);
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
	if (allowMethods.empty())
		return (1);
	this->_allowMethods = allowMethods;
	return (0);
}

int Location::setCgiPass(const std::string& value)
{
	std::stringstream ss(value);
	std::string dir;

	ss >> dir;
	if (dir.empty() || dir.at(0) != '/') // check if is path
		return (1);
	this->_cgiPass = dir;
	if (ss >> dir) // check if it has more text
		return (1);
	return (0);
}

int Location::setRedirect(const std::string& value)
{
	std::stringstream ss(value);
	int code;
	std::string uri;

	ss >> code;
	if (code != 301 && code != 302 && code != 303 && code != 307)
		return (1);
	ss >> uri;
	this->_redirect.first = code;
	this->_redirect.second = uri;
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

bool Location::hasRedirect()
{
	if (!this->_redirect.second.empty())
		return (true);
	return (false);
}

void Location::initializeMethods()
{
	if (!Location::_methods.empty())
		return;
	_methods["root"] = &CommonDirectives::setRoot;
	_methods["index"] = &CommonDirectives::setIndex;
	_methods["autoindex"] = &CommonDirectives::setAutoindex;
	_methods["upload_store"] = &CommonDirectives::setUploadStore;
	_methods["allow_methods"] = &Location::setAllowMethods;
	_methods["cgi_pass"] = &Location::setCgiPass;
	_methods["return"] = &Location::setRedirect;
}

int Location::setDirective(const std::string& directive,
						   const std::string& value)
{
	std::map<std::string, int (Location::*)(const std::string&)>::iterator
		it(_methods.find(directive));
	if (it != _methods.end())
		return ((this->*(it->second))(value));
	return (1);
}

