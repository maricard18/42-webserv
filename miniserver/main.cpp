/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 11:34:23 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/10/17 20:13:10 by maricard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cluster.hpp"

int main(int argc, char** argv)
{
	if (argc > 2)
	{
		std::cout << "Usage:\n\t./webserv [configuration_file]" << std::endl;
		return (1);
	}
	std::string config_path;
	Cluster cluster;

	if (argc == 2)
		config_path = argv[1];
	cluster.setup(config_path);
	cluster.run();
}
