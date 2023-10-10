#include "miniserver.hpp"

int main()
{
	int socketFd;
	struct sockaddr_in serverAddress;

	if ((socketFd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0) < 0))
		std::cerr << "Error creating socket" << std::endl;

	bzero(serv)
}
