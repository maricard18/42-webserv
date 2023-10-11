#ifndef MINISERVER_HPP
# define MINISERVER_HPP

#include <iostream>
#include <sys/select.h> // for select() function
#include <sys/socket.h> // for socket functions
#include <arpa/inet.h>  // For the inet_pton and inet_ntop functions
#include <netinet/in.h> // For the sockaddr_in structure and related constants
#include <string.h>
#include <unistd.h>

#define SERVER_PORT 8989

#endif
