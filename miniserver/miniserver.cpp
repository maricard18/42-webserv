#include "miniserver.hpp"

int main()
{
	int main_socket;
	struct sockaddr_in server_address;

	if ((main_socket = socket(AF_INET, SOCK_STREAM, 0) < 0))
	{
		std::cerr << "Error creating socket" << std::endl;
		return 0;
	}

	bzero(&server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(SERVER_PORT);

	fd_set current_sockets, ready_sockets;

	FD_ZERO(&current_sockets);
	FD_SET(main_socket, &current_sockets);

	//while (1)
	//{
	//	ready_sockets = current_sockets;

	//	if (select(FD_SETSIZE, &ready_sockets, &ready_sockets, NULL, NULL) < 0)
	//	{
	//		std::cerr << "Error selecting socket" << std::endl;
	//		return 0;
	//	}

	//	for (int i = 0; i < 2; i++)
	//	{
	//		if (FD_ISSET(i, &ready_sockets))
	//		{
	//			;
	//		}
	//	}
	//}

	if (bind(main_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
	{
		std::cerr << "Error binding socket" << std::endl;
		return 0;
	}

	if (listen(main_socket, 1) < 0)
	{
		std::cerr << "Error listening socket" << std::endl;
		return 0;
	}

}
