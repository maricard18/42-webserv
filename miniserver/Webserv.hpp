/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 12:41:18 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/10/14 15:25:29 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "Server.hpp"

class Webserv // TODO change to cluster
{
	const std::vector<Server> serverList;

public:
	Webserv();
	Webserv(const Webserv&);
	Webserv& operator=(const Webserv&);
	~Webserv();

	void setup(std::string config_file_path);
};
