/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 20:16:23 by maricard          #+#    #+#             */
/*   Updated: 2023/11/27 18:55:16 by maricard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Request.hpp"

std::string	getFileExtension(std::string& path);
uint32_t	getHexSize(char* buffer);
int			getHexFromChunked(char* buffer);
