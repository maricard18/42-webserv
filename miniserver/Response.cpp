/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 19:09:05 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/11/03 16:25:29 by bsilva-c         ###   ########.fr       */
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

	for (std::map<std::string, std::string>::iterator it = header.begin();
		 it != header.end(); ++it)
		response.append((*it).first + ": " + (*it).second + "\n");
	response.append(CRLF);
	for (std::vector<std::string>::iterator it = body.begin();
		 it != body.end(); ++it)
		response.append(*it);
	response.append(CRLF);
	return (response);
}
