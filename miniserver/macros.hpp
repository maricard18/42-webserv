/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macros.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 15:56:15 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/10/19 19:47:00 by maricard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iomanip>
#include <netdb.h>
#include <cstring>
#include <ctime>

#ifndef DEBUG
# define DEBUG true
#endif

#define HTML "HTTP/1.1 200 OK\r\n\r\n<html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>File Upload Example</title></head><body><h1>File Upload Example</h1><form action=\"/upload\" method=\"POST\" enctype=\"multipart/form-data\"><input type=\"file\" name=\"file\" required><button type=\"submit\">Upload File</button></form></body></html>"
#define HTML_GET "HTTP/1.1 200 OK\r\n\r\n<html lang=\"en\"><head>\n<meta charset=\"UTF-8\">\n<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Button Click Example</title>\n</head>\n\n<body>\n<h1>Button Click Example</h1>\n<button onclick=\"alert('Button Clicked!')\">Click Me</button>\n</body>\n\n</html>"

#ifndef COLORS_FOREGROUND
# define F_BLACK    "\033[1;30m"
# define F_RED    "\033[1;31m"
# define F_GREEN  "\033[1;32m"
# define F_YELLOW "\033[1;33m"
# define F_BLUE "\033[1;34m"
# define F_MAGENTA  "\033[1;35m"
# define F_CYAN  "\033[1;36m"
# define F_WHITE  "\033[1;37m"
#endif

#ifndef COLORS_BACKGROUND
# define B_BLACK    "\033[1;40m"
# define B_RED    "\033[1;41m"
# define B_GREEN  "\033[1;42m"
# define B_YELLOW "\033[1;43m"
# define B_BLUE "\033[1;44m"
# define B_MAGENTA  "\033[1;45m"
# define B_CYAN  "\033[1;46m"
# define B_WHITE  "\033[1;47m"
#endif

#ifndef RESET
# define RESET  "\033[0m"
#endif

#ifndef CRITICAL
# define CRITICAL 1
#endif

#ifndef WARNING
# define WARNING 2
#endif

#ifndef INFORMATION
# define INFORMATION 3
#endif

#ifndef MESSAGE
# define MESSAGE(message, level) {\
    if (DEBUG)\
    {\
        time_t current = std::time(0);\
        tm* time = std::localtime(&current);\
        if (level == CRITICAL)\
        {\
            std::cerr << B_RED "[" << std::setw(2) << std::setfill('0') <<\
            time->tm_hour << ":"  << std::setw(2) << std::setfill('0') <<\
            time->tm_min << ":" << std::setw(2) << std::setfill('0') <<\
            time->tm_sec << "]" << RESET F_RED " Error: ";\
        }\
        else if (level == WARNING)\
        {\
            std::cerr << B_YELLOW "[" << std::setw(2) << std::setfill('0') <<\
            time->tm_hour << ":"  << std::setw(2) << std::setfill('0') <<\
            time->tm_min << ":" << std::setw(2) << std::setfill('0') <<\
            time->tm_sec << "]" << RESET F_YELLOW " Warning: ";\
        }\
        else if (level == INFORMATION)\
        {\
            std::cerr << B_BLUE "[" << std::setw(2) << std::setfill('0') <<\
            time->tm_hour << ":"  << std::setw(2) << std::setfill('0') <<\
            time->tm_min << ":" << std::setw(2) << std::setfill('0') <<\
            time->tm_sec << "]" << RESET F_BLUE " Info: ";\
        }\
        std::cerr << message << RESET << std::endl;\
    }\
}
#endif
