# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    cgi_chunk.py                                       :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maricard <maricard@student.porto.com>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/18 17:24:25 by maricard          #+#    #+#              #
#    Updated: 2023/12/02 17:19:38 by maricard         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

import os, sys

uploads_folder = os.environ.get('UPLOAD_STORE')

if (uploads_folder != "uploads") :
    location = "(check configuration file)"
else :
    location = uploads_folder

# Check if the folder exists, if not create it
if not os.path.exists(uploads_folder):
    os.makedirs(uploads_folder)

message = ''

if 'CONTENT_TYPE' in os.environ and os.environ['CONTENT_TYPE'] == 'application/octet-stream':
    raw_data = sys.stdin.buffer.read()
    fn = 'output.bin'
    upload_path = os.path.join(uploads_folder, fn)

    # Check if the file already exists, if not create it
    if not os.path.exists(upload_path):
        with open(upload_path, 'wb') as new_file:
            new_file.write(raw_data)

        message = f'The file "{fn}" was uploaded successfully to /{location}'

    else:
        message = f'The file "{fn}" already exists'

else:
    message = 'No file was uploaded'

print (message)
