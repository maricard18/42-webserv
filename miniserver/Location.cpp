/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 13:01:17 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/10/15 17:03:05 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

// TODO better way to initialize w/ CommonDirectives
Location::Location()
{
}

Location::Location(const std::vector<std::string>& allow_methods,
				   const std::string& upload_store,
				   const std::string& cgi_pass)
	: allow_methods(allow_methods),
	  upload_store(upload_store),
	  cgi_pass(cgi_pass)
{
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

std::vector<std::string> Location::getAllowMethods() const
{
	return (this->allow_methods);
}

std::string Location::getUploadStore() const
{
	return (this->upload_store);
}

std::string Location::getCgiPass() const
{
	return (this->cgi_pass);
}
