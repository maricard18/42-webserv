/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 19:09:05 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/11/06 13:31:25 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response()
{
}

Response::Response(const Response&)
{
}

Response& Response::operator=(const Response&)
{
	return (*this);
}

Response::~Response()
{
}

std::string Response::buildResponse(std::map<std::string, std::string>& header,
									std::vector<std::string>& body)
{
	std::string response;

	response.append("HTTP/1.1 " + header["HTTP/1.1"] + "\n");
	for (std::map<std::string, std::string>::iterator it = header.begin();
		 it != header.end(); ++it)
	{
		if ((*it).first != "HTTP/1.1")
			response.append((*it).first + ": " + (*it).second + "\n");
	}
	response.append("Server: Webserv (Unix)\n");
	response.append(CRLF);
	if (!body.empty())
	{
		for (std::vector<std::string>::iterator it = body.begin();
			 it != body.end(); ++it)
			response.append(*it);
		response.append(CRLF);
	}
	if (header["HTTP/1.1"] != "No Content" && body.empty())
		MESSAGE("500 Internal Server Error", WARNING);
	return (response);
}
