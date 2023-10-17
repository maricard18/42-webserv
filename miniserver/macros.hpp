/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macros.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 15:56:15 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/10/17 19:36:12 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef DEBUG
# define DEBUG true
#endif

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

#ifndef ERROR
# define ERROR(message, level) { \
    if (!DEBUG) {return 0;}\
    if (level == CRITICAL)\
    {\
        std::cerr << B_RED << "Error: ";\
    }\
    else if (level == WARNING)\
    {\
        std::cerr << B_YELLOW << "Warning: ";\
    }\
    else if (level == INFORMATION)\
    {\
        std::cerr << B_BLUE << "Info: ";\
    }\
    std::cerr << message << RESET << std::endl;\
}
#endif
