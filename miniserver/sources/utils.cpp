/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 20:09:21 by maricard          #+#    #+#             */
/*   Updated: 2023/11/25 18:32:05 by maricard         ###   ########.fr       */
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

int countDigits(int number) {
    int count = 0;

    // Handle the case when the number is 0 separately
    if (number == 0) {
        return 1;
    }

    // For negative numbers, remove the sign temporarily
    if (number < 0) {
        number = -number;
    }

    // Count the digits using a loop
    while (number != 0) {
        number /= 10;
        ++count;
    }

    return count;
}
