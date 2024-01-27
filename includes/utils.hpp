/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 20:16:23 by maricard          #+#    #+#             */
/*   Updated: 2024/01/27 17:51:06 by maricard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Request.hpp"

std::string	getFileExtension(std::string& path);
uint32_t	getHexSize(const std::vector<char>& body, unsigned pos);
uint32_t	getHexFromChunked(const std::vector<char>& body, unsigned pos);
bool		searchChunkedRequestEnd(const std::vector<char>& body);
