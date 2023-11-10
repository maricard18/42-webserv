/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 13:01:17 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/11/09 20:30:21 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

std::map<std::string, int (Location::*)(const std::string&)> Location::_methods;

Location::Location()
	: CommonDirectives()
{
	Location::initializeMethods();
}

Location::Location(const std::string& path)
	: CommonDirectives(), _path(path), _cgiPass(path)
{
	Location::initializeMethods();
}

Location::Location(const Location& value)
	: CommonDirectives(value._root,
					   value._index,
					   value._uploadStore,
					   value._autoindex),
	  _path(value._path),
	  _allowMethods(value._allowMethods),
	  _cgiPass(value._cgiPass),
	  _redirect(value._redirect)
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

std::string Location::getCgiPass(Server& server) const
{
	if (this->_cgiPass.empty())
	{
		std::string path = this->_path;
		Location* location = server.getParentLocation(path);
		if (location)
			return (location->getCgiPass(server));
	}
	return (this->_cgiPass);
}

std::pair<std::string, std::string>& Location::getRedirect(Server& server)
{
	if (this->_redirect.first.empty())
	{
		std::string path = this->_path;
		Location* location = server.getParentLocation(path);
		if (location)
			return (location->getRedirect(server));
	}
	return (this->_redirect);
}

std::string Location::getRoot(Server& server) const
{
	if (this->_root.empty())
		return (server.getRoot());
	return (this->_root);
}

std::vector<std::string> Location::getIndex(Server& server) const
{
	if (this->_index.empty())
	{
		std::string path = this->_path;
		Location* location = server.getParentLocation(path);
		if (location)
			return (location->getIndex(server));
		else
			return (server.getIndex());
	}
	return (this->_index);
}

std::string Location::getUploadStore(Server& server) const
{
	if (this->_uploadStore.empty())
	{
		std::string path = this->_path;
		Location* location = server.getParentLocation(path);
		if (location)
			return (location->getUploadStore(server));
		else
			return (server.getUploadStore());
	}
	return (this->_uploadStore);
}

std::string Location::getPath() const
{
	return (this->_path);
}

int Location::setAllowMethods(const std::string& value)
{
	std::stringstream ss(value);
	std::string method;

	while (ss >> method)
	{
		if (method == "GET" || method == "POST" || method == "DELETE")
		{
			for (std::vector<std::string>::iterator
					 it = this->_allowMethods.begin();
				 it != this->_allowMethods.end(); ++it)
				if (*it == method)
					return (1);
			this->_allowMethods.push_back(method);
		}
		else
			return (1);
	}
	if (this->_allowMethods.empty())
		return (1);
	return (0);
}

int Location::setCgiPass(const std::string& value)
{
	std::stringstream ss(value);
	std::string dir;

	ss >> dir;
	if (dir.empty() || dir.at(0) != '/' ||
		dir.find("//") != std::string::npos) // check if is path
		return (1);
	this->_cgiPass = dir;
	if (*this->_cgiPass.end() == '/')
		*this->_cgiPass.end() = '\0';
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
	std::stringstream ssCode;
	ssCode << code;
	this->_redirect = std::make_pair(ssCode.str(), uri);
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

bool Location::hasRedirect(Server& server)
{
	if (!this->getRedirect(server).second.empty())
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

