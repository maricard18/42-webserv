/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 20:09:21 by maricard          #+#    #+#             */
/*   Updated: 2023/11/29 20:29:50 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

std::string getFileExtension(std::string& path)
{
	uint64_t dotIndex = path.find_last_of('.');
	
	if (dotIndex != std::string::npos)
		return path.substr(dotIndex, path.length() - dotIndex);
	return "";
}


uint32_t	getHexSize(char* buffer)
{
	uint32_t pos = 0;
//	std::cout << "buffer: " << buffer[0] << std::endl;
	while (buffer + pos && std::strncmp(buffer + pos, CRLF, 2) != 0)
		pos++;
	return (pos);
}

int getHexFromChunked(char* buffer)
{
	std::string hex = buffer;

	hex = hex.substr(0, getHexSize(buffer));

	std::stringstream ss(hex);
	int value;

	ss >> std::hex >> value;
	return (value);
}
