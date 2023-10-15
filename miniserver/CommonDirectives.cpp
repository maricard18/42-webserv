/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonDirectives.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 13:02:38 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/10/15 21:58:36 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommonDirectives.hpp"

CommonDirectives::CommonDirectives()
	: autoindex(false)
{
}

CommonDirectives::CommonDirectives(const std::string& root)
	: root(root)
{
}

CommonDirectives::CommonDirectives(const std::string& root, bool autoindex)
	: root(root), autoindex(autoindex)
{
}

CommonDirectives::CommonDirectives(const std::string& root,
								   const std::vector<std::string>& index,
								   bool autoindex)
	: root(root), index(index), autoindex(autoindex)
{
}

CommonDirectives::CommonDirectives(const CommonDirectives& value)
	: root(value.root), index(value.index), autoindex(value.autoindex)
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
	return (this->root);
}

std::vector<std::string> CommonDirectives::getIndex() const
{
	return (this->index);
}

bool CommonDirectives::getAutoindex() const
{
	return (this->autoindex);
}

int CommonDirectives::setRoot(const std::string& value)
{
	std::stringstream ss(value);
	std::string dir;

	ss >> dir;
	if (dir.at(0) != '/') // check if is path
		return (1);
	this->root = dir;
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
			this->index.push_back(method);
		else
			return (1);
	}
	return (0);
}

int CommonDirectives::setAutoindex(const std::string& value)
{
	if (value == "true" || value == "TRUE")
		this->autoindex = true;
	else if (value == "false" || value == "FALSE")
		this->autoindex = false;
	else
		return (1);
	return (0);
}
