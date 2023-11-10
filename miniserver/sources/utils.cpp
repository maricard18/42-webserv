/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 20:09:21 by maricard          #+#    #+#             */
/*   Updated: 2023/11/10 20:51:17 by maricard         ###   ########.fr       */
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