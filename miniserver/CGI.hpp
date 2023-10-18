/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 17:39:30 by maricard          #+#    #+#             */
/*   Updated: 2023/10/18 17:56:07 by maricard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include "macros.hpp"
#include "Request.hpp"

class CGI
{
	private:
		char **_argv;
		char **_envp;

	public:
		CGI();
		CGI(Request &request);
		CGI(const CGI& copy);
		~CGI();
		CGI& operator=(const CGI& other);
}