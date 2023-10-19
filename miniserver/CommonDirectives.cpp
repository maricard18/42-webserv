/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonDirectives.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 13:02:38 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/10/18 17:53:47 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommonDirectives.hpp"

CommonDirectives::CommonDirectives()
	: _root("/"), _uploadStore(this->_root), _autoindex(false)
{
	this->_index.push_back("index.htm");
	this->_index.push_back("index.html");
	this->_index.push_back("index.php");
}

CommonDirectives::CommonDirectives(const std::string& root)
	: _root(root), _uploadStore(this->_root), _autoindex(false)
{
	this->_index.push_back("index.htm");
	this->_index.push_back("index.html");
	this->_index.push_back("index.php");
}

CommonDirectives::CommonDirectives(const std::string& root,
								   const std::vector<std::string>& index,
								   const std::string& upload_store,
								   bool autoindex)
	: _root(root),
	  _index(index),
	  _uploadStore(upload_store),
	  _autoindex(autoindex)
{
}

CommonDirectives::CommonDirectives(const CommonDirectives& value)
	: _root(value._root),
	  _index(value._index),
	  _uploadStore(value._uploadStore),
	  _autoindex(value._autoindex)
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

std::string CommonDirectives::getUploadStore() const
{
	return (this->_uploadStore);
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
	std::vector<std::string> indexes;
	std::stringstream ss(value);
	std::string file;

	while (ss >> file)
	{
		if (file.find_first_of('.') != std::string::npos &&
			file.find_first_of('.') == file.find_last_of('.') &&
			*file.begin() != '.' && *file.end() != '.')
			indexes.push_back(file);
		else
			return (1);
	}
	this->_index = indexes;
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

int CommonDirectives::setUploadStore(const std::string& value)
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
