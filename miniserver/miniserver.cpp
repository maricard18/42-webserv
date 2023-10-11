#include "miniserver.hpp"

int main()
{
	int main_socket;
	struct sockaddr_in server_address;

	// to get an non block socket use <SOCK_STREAM | SOCK_NONBLOCK> as second argument
	if ((main_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
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

	int connection;
	auto address_length = sizeof(server_address);

	std::cout << "starting the connection between server and client" << std::endl;

	if ((connection = accept(main_socket, (struct sockaddr*)&server_address, (socklen_t*)&address_length)) < 0)
	{
		std::cerr << "Failed to grab connection." << std::endl
				  << "errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << "server and client connected successfully!" << std::endl;

	char buffer[100];
	auto bytesRead = read(connection, buffer, 100);
	std::cout << "The message was: " << buffer;

	std::string response = "Good talking to you\n";
	send(connection, response.c_str(), response.size(), 0);

	close(connection);
	close(main_socket);
}
