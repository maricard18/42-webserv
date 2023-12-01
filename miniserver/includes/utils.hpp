/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 20:16:23 by maricard          #+#    #+#             */
/*   Updated: 2023/12/01 18:12:47 by maricard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Request.hpp"

std::string	getFileExtension(std::string& path);
uint32_t	getHexSize(const std::vector<char> body, unsigned pos);
uint32_t	getHexFromChunked(const std::vector<char> body, unsigned pos);
