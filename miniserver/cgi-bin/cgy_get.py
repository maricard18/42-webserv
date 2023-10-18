# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    cgy_get.py                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maricard <maricard@student.porto.com>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/18 17:24:21 by maricard          #+#    #+#              #
#    Updated: 2023/10/18 18:14:23 by maricard         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

from datetime import date

# get current date and time, time not parsed
date_and_time = date.now()

# parse to format dd/mm/YY H:M:S
parsed_value = date_and_time.strftime("%d/%m/%Y %H:%M:%S")

# print parsed value
print(parsed_value)