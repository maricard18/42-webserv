/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 12:41:04 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/10/13 13:45:13 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

Webserv::Webserv()
{
}

Webserv::Webserv(const Webserv& value)
	: serverList(value.serverList)
{
}

Webserv& Webserv::operator=(const Webserv&)
{
	// TODO
	return (*this);
}

Webserv::~Webserv()
{
}

void Webserv::setup(std::string config_file_path)
{
	// TODO Open config file and update values
}
