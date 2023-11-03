# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maricard <maricard@student.porto.com>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/15 15:11:41 by bsilva-c          #+#    #+#              #
#    Updated: 2023/11/03 16:26:13 by bsilva-c         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

RES	= \033[0m
RED	= \033[1;31m
GRE	= \033[1;32m
YEL	= \033[1;33m
CYA	= \033[1;36m
WHI	= \033[1;37m

CXX=c++
CXXFLAGS=-Wall -Werror -Wextra -g -std=c++98
DEPS=

SRCS=main.cpp Cluster.cpp Server.cpp Location.cpp CommonDirectives.cpp \
Response.cpp Request.cpp
OBJS=$(SRCS:.cpp=.o)
NAME=webserv

all: $(NAME)
$(NAME): $(OBJS) $(DEPS)
	@$(CXX) $(OBJS) $(DEPS) -o $(NAME)
clean:
	@echo "$(WHI)[$(RED)Cleaned$(WHI)]$(RES)"
	@rm $(OBJS)
fclean: clean
	@echo "$(WHI)[$(RED)Cleaned even better$(WHI)]$(RES)"
	@rm $(NAME)
re: fclean all
run: re 
	@clear
	@echo "$(WHI)[$(GRE)running$(WHI)]$(RES)"
	./$(NAME)
