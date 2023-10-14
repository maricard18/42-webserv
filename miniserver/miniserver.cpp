/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniserver.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 11:34:23 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/10/13 13:52:07 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniserver.hpp"
#include "Webserv.hpp"

int main()
{
	const int trueFlag = 1;
	int _socket[2];
	struct sockaddr_in server_address[2];

	// to get an non block socket use <SOCK_STREAM | SOCK_NONBLOCK> as second argument
	if ((_socket[0] = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) < 0)
	{
		std::cerr << "Error creating socket." << std::endl
				  << "errno: " << errno << std::endl;
		return 0;
	}
	if (setsockopt(_socket[0], SOL_SOCKET, SO_REUSEADDR, &trueFlag, sizeof(int)) < 0)
	{
		std::cerr << "Error setting up socket." << std::endl
				  << "errno: " << errno << std::endl;
		return 0;
	}
	// to get an non block socket use <SOCK_STREAM | SOCK_NONBLOCK> as second argument
	if ((_socket[1] = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) < 0)
	{
		std::cerr << "Error creating socket." << std::endl
				  << "errno: " << errno << std::endl;
		return 0;
	}
	if (setsockopt(_socket[1], SOL_SOCKET, SO_REUSEADDR, &trueFlag, sizeof(int)) < 0)
	{
		std::cerr << "Error setting up socket." << std::endl
				  << "errno: " << errno << std::endl;
		return 0;
	}

	bzero(&server_address[0], sizeof(server_address[0]));
	server_address[0].sin_family = AF_INET;
	server_address[0].sin_addr.s_addr = INADDR_ANY;
	server_address[0].sin_port = htons(SERVER_PORT);
	std::cout << "SERVER PORT = " << SERVER_PORT << std::endl;
	bzero(&server_address[1], sizeof(server_address[1]));
	server_address[1].sin_family = AF_INET;
	server_address[1].sin_addr.s_addr = INADDR_ANY;
	server_address[1].sin_port = htons(4343);
	std::cout << "SERVER PORT = " << 4343 << std::endl;

	if (bind(_socket[0], (struct sockaddr *)&server_address[0], sizeof(server_address[0])) < 0)
	{
		std::cerr << "Error binding socket." << std::endl
				  << "errno: " << errno << std::endl;
		return 0;
	}
	if (bind(_socket[1], (struct sockaddr *)&server_address[1], sizeof(server_address[1])) < 0)
	{
		std::cerr << "Error binding socket." << std::endl
				  << "errno: " << errno << std::endl;
		return 0;
	}

	if (listen(_socket[0], 1) < 0)
	{
		std::cerr << "Error listening socket." << std::endl
				  << "errno: " << errno << std::endl;
		return 0;
	}
	if (listen(_socket[1], 1) < 0)
	{
		std::cerr << "Error listening socket." << std::endl
				  << "errno: " << errno << std::endl;
		return 0;
	}

	fd_set current_sockets, ready_sockets;

	FD_ZERO(&current_sockets);
	FD_SET(_socket[0], &current_sockets);
	FD_SET(_socket[1], &current_sockets);

	std::cout << "starting the connection between server and client" << std::endl;

	while (1)
	{
		ready_sockets = current_sockets;

		if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0)
		{
			std::cerr << "Error on select." << std::endl
				  	  << "errno: " << errno << std::endl;
			return 0;
		}

		int connection = -1;
		if (FD_ISSET(_socket[0], &ready_sockets))
		{
			auto address_length = sizeof(server_address[1]);
			if ((connection = accept(_socket[0], (struct sockaddr*)&server_address[0], (socklen_t*)&address_length)) < 0)
			{
				std::cerr << "Error on select." << std::endl
						  << "errno: " << errno << std::endl;
				continue ;
			}
		}
		if (FD_ISSET(_socket[1], &ready_sockets))
		{
			auto address_length = sizeof(server_address[0]);
			if ((connection = accept(_socket[1], (struct sockaddr*)&server_address[1], (socklen_t*)&address_length)) < 0)
			{
				std::cerr << "Error on select." << std::endl
						  << "errno: " << errno << std::endl;
				continue ;
			}
		}
		
		std::cout << std::endl << "server and client connected successfully!" << std::endl;

		char buffer[10000];
		auto bytesRead = read(connection, buffer, 10000);
		if (bytesRead == -1)
		{
			close(connection);
			continue ;
		}
		std::string input = buffer;
		std::cout << "The message was: " << input << std::endl;

		std::string response = "HTTP/1.1 200 OK\r\n\r\nHello how are you?\n\nI am the server\n";
		send(connection, response.c_str(), response.size(), 0);

//		close(connection);
	}

	close(_socket[0]);
	close(_socket[1]);
}
