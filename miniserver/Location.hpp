/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 13:01:23 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/10/13 13:44:54 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommonDirectives.hpp"
#include <iostream>
#include <string>
#include <vector>

#pragma once

class Location : public CommonDirectives
{
	const std::vector<std::string> allow_methods;
	const std::string upload_store;
	const std::string cgi_pass;

public:
	Location();
	Location(const Location&);
	Location& operator=(const Location&);
	~Location();

	std::vector<std::string> getAllowMethods() const;
	std::string getCgiPass() const;
};
