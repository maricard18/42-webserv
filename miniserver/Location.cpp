/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 13:01:17 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/10/13 13:44:49 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location()
{
}

Location::Location(const Location& value)
	: allow_methods(value.allow_methods), cgi_pass(value.cgi_pass)
{
}

Location& Location::operator=(const Location&)
{
	if (*this == value)
		return (*this);
	// TODO
	return (*this);
}

Location::~Location()
{
}

std::vector<std::string> Location::getAllowMethods() const
{
	return (this->allow_methods);
}

std::string Location::getCgiPass() const
{
	return (this->cgi_pass);
}
