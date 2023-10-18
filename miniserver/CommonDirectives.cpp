/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonDirectives.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 13:02:38 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/10/18 16:22:49 by maricard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommonDirectives.hpp"

CommonDirectives::CommonDirectives()
	: _root("/"), _autoindex(false)
{
	this->_index.push_back("index.htm");
	this->_index.push_back("index.html");
	this->_index.push_back("index.php");
}

CommonDirectives::CommonDirectives(const std::string& root)
	: _root(root), _autoindex(false)
{
	this->_index.push_back("index.htm");
	this->_index.push_back("index.html");
	this->_index.push_back("index.php");
}

CommonDirectives::CommonDirectives(const std::string& root,
								   const std::vector<std::string>& index,
								   bool autoindex)
	: _root(root), _index(index), _autoindex(autoindex)
{
}

CommonDirectives::CommonDirectives(const CommonDirectives& value)
	: _root(value._root), _index(value._index), _autoindex(value._autoindex)
{
}

CommonDirectives& CommonDirectives::operator=(const CommonDirectives& value)
{
	if (this == &value)
		return (*this);

	CommonDirectives tmp(value);

	std::swap(*this, tmp);
	return (*this);
}

CommonDirectives::~CommonDirectives()
{
}

std::string CommonDirectives::getRoot() const
{
	return (this->_root);
}

std::vector<std::string> CommonDirectives::getIndex() const
{
	return (this->_index);
}

bool CommonDirectives::getAutoindex() const
{
	return (this->_autoindex);
}

int CommonDirectives::setRoot(const std::string& value)
{
	std::stringstream ss(value);
	std::string dir;

	ss >> dir;
	if (dir.at(0) != '/') // check if is path
		return (1);
	this->_root = dir;
	if (ss >> dir) // check if it has more text
		return (1);
	return (0);
}

int CommonDirectives::setIndex(const std::string& value)
{
	std::stringstream ss(value);
	std::string method;

	while (ss >> method)
	{
		if (method.find_first_of('.') != std::string::npos &&
			method.find_first_of('.') == method.find_last_of('.') &&
			*method.begin() != '.' && *method.end() != '.')
			this->_index.push_back(method);
		else
			return (1);
	}
	return (0);
}

int CommonDirectives::setAutoindex(const std::string& value)
{
	if (value == "true" || value == "TRUE")
		this->_autoindex = true;
	else if (value == "false" || value == "FALSE")
		this->_autoindex = false;
	else
		return (1);
	return (0);
}
