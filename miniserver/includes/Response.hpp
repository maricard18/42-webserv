/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 19:09:15 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/11/11 10:09:54 by maricard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <vector>
#include <sstream>
#include <iostream>
#include "macros.hpp"

class Response
{
	std::map<std::string, std::string> _header;
	std::vector<char> _body;
	static std::map<std::string, std::string> _errorStatus;
	static std::map<std::string, std::string> _redirStatus;
	static std::map<std::string, std::string> _contentType;

public:
	Response();
	Response(const Response&);
	Response& operator=(const Response&);
	~Response();

	static std::string buildResponse(std::map<std::string, std::string>& header,
									 std::string extension,
									 std::vector<char>& body);
	static std::string buildErrorResponse(int errorCode);
	static std::string buildRedirectResponse(const std::pair<std::string,
															 std::string>& redirect);
	static void initializeErrorStatus();
	static void initializeRedirStatus();
	static void initializeContentType();
};
