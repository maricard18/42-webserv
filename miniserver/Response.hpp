/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 19:09:15 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/10/30 19:44:20 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <vector>
#include <iostream>
#include "macros.hpp"

class Response
{
	std::map<std::string, std::string> _header;
	std::vector<std::string> _body;
public:
	Response();
	Response(const Response&);
	Response& operator=(const Response&);
	~Response();

	std::string buildResponse(std::map<std::string, std::string>& header,
							  std::vector<std::string>& body);
};
