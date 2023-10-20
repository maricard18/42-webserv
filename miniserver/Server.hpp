/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 12:51:53 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/10/20 21:37:00 by maricard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "CommonDirectives.hpp"
#include "Request.hpp"
#include "Location.hpp"
#include <netinet/in.h>
#include <sys/socket.h>

class Server : public CommonDirectives
{
	std::vector<std::string> _serverNames;
	std::string _address;
	u_int16_t _listen;
	u_int32_t _clientMaxBodySize; // in MiB
	std::map<int, std::string> _errorPage;
	std::map<std::string, Location*> _locations;

	int32_t _socket;
	struct sockaddr_in _serverAddress;

	static std::map<std::string, int (Server::*)(const std::string&)> _methods;
	static void initializeMethods();
	int setAddress(const std::string& value);
public:
	Server();
	Server(const Server&);
	Server& operator=(const Server&);
	~Server();

	std::vector<std::string> getServerNames() const;
	std::string getAddress() const;
	u_int16_t getListenPort() const;
	u_int32_t getClientMaxBodySize() const;
	std::string getErrorPage(int error_code);
	Location& getLocation(const std::string& location);

	int32_t getSocket() const;
	const sockaddr_in& getServerAddress() const;

	int setServerNames(const std::string& value);
	int setListen(const std::string& value);
	int setClientMaxBodySize(const std::string& value);
	int setErrorPage(const std::string& value);
	int setLocation(const std::string& dir, Location* value);

	int setDirective(const std::string& directive, const std::string& value);

	std::string	handleRequest(const std::string& buffer);

	int run();
	void stop();
};
