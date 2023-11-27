/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 20:09:21 by maricard          #+#    #+#             */
/*   Updated: 2023/11/27 20:32:55 by maricard         ###   ########.fr       */
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

int countDigits(int number)
{
    int count = 0;

    if (number == 0)
        return 1;

    if (number < 0)
        number = -number;

    while (number != 0)
	{
        number /= 10;
        ++count;
    }

    return count;
}

int getDigit(int pos, char* string)
{
	int k = pos;

	while (string[k] && std::isdigit(string[k]))
		k++;

	return k;
}
