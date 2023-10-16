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
#include <netinet/in.h>
#include <sys/socket.h>
#include <map>

class Server : public CommonDirectives
{
	std::vector<std::string> _serverNames;
	in_addr_t _address;
	const u_int16_t _listen;
	u_int32_t _clientMaxBodySize; // in MiB
	std::map<int, std::string> _errorPage;
	std::map<std::string, Location*> _locations;
	std::string _uploadStore;

	int32_t _socket;
	struct sockaddr_in _serverAddress;

	Server();
public:
	Server(const std::string& root,
		   const std::vector<std::string>& index,
		   u_int16_t listen);
	Server(const Server&);
	Server& operator=(const Server&);
	~Server();

	std::vector<std::string> getServerNames() const;
	in_addr_t getAddress() const;
	u_int16_t getListenPort() const;
	u_int32_t getClientMaxBodySize() const;
	std::string getErrorPage(int error_code);
	Location& getLocation(const std::string& location);
	int32_t getSocket() const;
	const sockaddr_in& getServerAddress() const;
	std::string getUploadStore() const;

	int setServerNames(const std::string& value);
	int setAddress(const std::string& value);
	int setClientMaxBodySize(const std::string& value);
	int setErrorPage(const std::string& value);
	int setLocation(const std::string& dir, const Location& value);
	int setUploadStore(const std::string& value);

	void run();
};
