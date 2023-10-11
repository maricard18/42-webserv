#include "miniserver.hpp"

int main()
{
	int main_socket;
	struct sockaddr_in server_address;

	// to get an non block socket use <SOCK_STREAM | SOCK_NONBLOCK> as second argument
	if ((main_socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) < 0)
	{
		std::cerr << "Error creating socket." << std::endl
				  << "errno: " << errno << std::endl;
		return 0;
	}

	bzero(&server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(SERVER_PORT);
	std::cout << "SERVER PORT = " << SERVER_PORT << std::endl;

	if (bind(main_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
	{
		std::cerr << "Error binding socket." << std::endl
				  << "errno: " << errno << std::endl;
		return 0;
	}

	if (listen(main_socket, 1) < 0)
	{
		std::cerr << "Error listening socket." << std::endl
				  << "errno: " << errno << std::endl;
		return 0;
	}

	fd_set current_sockets, ready_sockets;

	FD_ZERO(&current_sockets);
	FD_SET(main_socket, &current_sockets);

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

		int connection;
		auto address_length = sizeof(server_address);
		
		if ((connection = accept(main_socket, (struct sockaddr*)&server_address, (socklen_t*)&address_length)) < 0)
		{
			continue ;
		}
		
		std::cout << std::endl << "server and client connected successfully!" << std::endl;

		char buffer[10000];
		auto bytesRead = read(connection, buffer, 10000);
		std::string input = buffer;
		std::cout << "The message was: " << buffer << std::endl;

		std::string response = "HTTP/1.1 200 OK\r\n\r\nHello how are you?\n\nI am the server\n";
		send(connection, response.c_str(), response.size(), 0);

		close(connection);
	}

	close(main_socket);
}
