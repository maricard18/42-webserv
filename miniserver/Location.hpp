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

#pragma once

#include "CommonDirectives.hpp"

class Location : public CommonDirectives
{
	std::vector<std::string> allow_methods;
	std::string upload_store;
	std::string cgi_pass;

	Location();
public:
	Location(const std::string& path);
	Location(const Location&);
	Location& operator=(const Location&);
	~Location();

	std::string getUploadStore() const;
	std::string getCgiPass() const;

	int setAllowMethods(const std::string& value);
	int setUploadStore(const std::string& value);
	int setCgiPass(const std::string& value);

	bool isMethodAllowed(const std::string& method);
};
