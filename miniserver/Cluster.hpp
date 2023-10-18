/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 12:41:18 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/10/17 20:02:40 by maricard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include "Request.hpp"
#include <sys/select.h>
#include <unistd.h>

class Cluster
{
	std::vector<Server> _serverList;

public:
	Cluster();
	Cluster(const Cluster&);
	Cluster& operator=(const Cluster&);
	~Cluster();

	void setup(const std::string& config_file_path);
	void run();
};
