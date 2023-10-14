/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonDirectives.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 13:02:38 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/10/14 15:38:41 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommonDirectives.hpp"

CommonDirectives::CommonDirectives()
	: autoindex(true)
{
}

CommonDirectives::CommonDirectives(const CommonDirectives& value)
	: root(value.root), index(value.index), autoindex(value.autoindex)
{
}

CommonDirectives& CommonDirectives::operator=(const CommonDirectives&)
{
	if (*this == value)
		return (*this);
	// TODO
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
