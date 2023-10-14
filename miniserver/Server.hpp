/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 12:51:53 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/10/13 13:45:07 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "CommonDirectives.hpp"
#include "Location.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <map>

class Server : public CommonDirectives
{
	const std::vector<std::string> server_names;
	const int listen;
	const int client_max_body_size; // in MiB
	const std::map<int, std::string> error_page;
	const std::map <std::string, Location> locations;

public:
	Server();
	Server(const Server&);
	Server& operator=(const Server&);
	~Server();

	std::vector<std::string> getServerNames() const;
	std::string getListenIP() const;
	int getListenPort() const;
	int getClientMaxBodySize() const;
	std::map<std::string, Location> getLocations() const;
};
