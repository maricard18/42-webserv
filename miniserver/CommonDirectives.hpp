/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonDirectives.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 13:02:45 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/10/14 15:28:38 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <vector>

class CommonDirectives
{
protected:
	const std::string root;
	const std::vector<std::string> index;
	const bool autoindex;

public:
	CommonDirectives();
	CommonDirectives(const CommonDirectives&);
	CommonDirectives& operator=(const CommonDirectives&);
	~CommonDirectives();

	std::string getRoot() const;
	std::vector<std::string> getIndex() const;
	bool getAutoindex() const;
};
