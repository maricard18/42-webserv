/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 13:01:23 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/10/20 15:33:34 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "CommonDirectives.hpp"

class Location : public CommonDirectives
{
	std::vector<std::string> _allowMethods;
	std::string _cgiPass;
	std::pair<int, std::string> _redirect;

	static std::map<std::string, int (Location::*)(const std::string&)>
		_methods;

	Location();
	static void initializeMethods();
public:
	Location(const std::string& path);
	Location(const Location&);
	Location& operator=(const Location&);
	~Location();

	std::string getCgiPass() const;
	std::pair<int, std::string> getRedirect() const;

	int setAllowMethods(const std::string& value);
	int setCgiPass(const std::string& value);
	int setRedirect(const std::string& value);

	bool isMethodAllowed(const std::string& method);
	bool hasRedirect();

	int setDirective(const std::string& directive, const std::string& value);

};
