/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 13:01:23 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/11/04 16:11:25 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "CommonDirectives.hpp"
#include "Server.hpp"

class Server;

class Location : public CommonDirectives
{
	std::string _path;
	std::vector<std::string> _allowMethods;
	std::string _cgiPass;
	std::pair<int, std::string> _redirect;

	static std::map<std::string, int (Location::*)(const std::string&)>
		_methods;

	Location();
	static void initializeMethods();
public:
	Location(const std::string& path);
	Location(const Location&);
	Location& operator=(const Location&);
	~Location();

	std::string getCgiPass(Server& server) const;
	std::pair<int, std::string> getRedirect(Server& server) const;
	std::string getRoot(Server& server) const;
	std::vector<std::string> getIndex(Server& server) const;
	std::string getUploadStore(Server& server) const;

	int setAllowMethods(const std::string& value);
	int setCgiPass(const std::string& value);
	int setRedirect(const std::string& value);

	bool isMethodAllowed(const std::string& method);
	bool hasRedirect() const;

	int setDirective(const std::string& directive, const std::string& value);

};
