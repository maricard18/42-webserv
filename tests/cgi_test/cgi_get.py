# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    cgi_get.py                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maricard <maricard@student.porto.com>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/18 17:24:21 by maricard          #+#    #+#              #
#    Updated: 2023/10/21 16:12:20 by maricard         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

from datetime import datetime as date

# get current date and time, time not parsed
date_and_time = date.now()

# parse to format dd/mm/YY H:M:S
parsed_value = date_and_time.strftime("%d/%m/%Y %H:%M:%S")

# print parsed value
print(parsed_value)