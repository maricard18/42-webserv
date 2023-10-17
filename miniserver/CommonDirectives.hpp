/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonDirectives.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 13:02:45 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/10/17 19:56:01 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "macros.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class CommonDirectives
{
	CommonDirectives();
protected:
	std::string _root;
	std::vector<std::string> _index;
	bool _autoindex;

	CommonDirectives(const std::string& root);
	CommonDirectives(const std::string& root,
					 const std::vector<std::string>& index,
					 bool autoindex);
public:
	CommonDirectives(const CommonDirectives&);
	CommonDirectives& operator=(const CommonDirectives&);
	~CommonDirectives();

	std::string getRoot() const;
	std::vector<std::string> getIndex() const;
	bool getAutoindex() const;

	int setRoot(const std::string& value);
	int setIndex(const std::string& value);
	int setAutoindex(const std::string& value);
};
