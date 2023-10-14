/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniserver.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 11:40:17 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/10/13 12:16:11 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISERVER_HPP
# define MINISERVER_HPP

#include <iostream>
#include <sys/select.h> // for select() function
#include <sys/socket.h> // for socket functions
#include <arpa/inet.h>  // For the inet_pton and inet_ntop functions
#include <netinet/in.h> // For the sockaddr_in structure and related constants
#include <string.h>
#include <unistd.h>

#define SERVER_PORT 8181

#endif
