/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 19:09:15 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/11/06 17:29:14 by bsilva-c         ###   ########.fr       */
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
	std::vector<std::string> _body;
	static std::map<std::string, std::string> _errorStatus;
public:
	Response();
	Response(const Response&);
	Response& operator=(const Response&);
	~Response();

	static std::string buildResponse(std::map<std::string, std::string>& header,
									 std::vector<std::string>& body);
	static std::string buildErrorResponse(int errorCode);
	static void initializeErrorStatus();
};