# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    cgy_post.py                                        :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maricard <maricard@student.porto.com>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/18 17:24:25 by maricard          #+#    #+#              #
#    Updated: 2023/10/18 17:24:26 by maricard         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

import cgi, os
import cgitb; cgitb.enable()

uploads_folder = os.environ.get('UPLOAD_STORE')
print(uploads_folder)

# Check if the folder exists, if not create it
if not os.path.exists(uploads_folder):
    os.makedirs(uploads_folder)

message = ''

# create an object to parse request
form = cgi.FieldStorage()

# parse field filename from request
fileitem = form['filename']

# Test if the file was uploaded
if fileitem.filename:
    
	# strip leading path from file name
    fn = os.path.basename(fileitem.filename)
    upload_path = os.path.join(uploads_folder, fn)
    
    # Check if the file already exists, if not create it
    if not os.path.exists(upload_path):
        
        with open(upload_path, 'wb') as new_file:
            new_file.write(fileitem.file.read())
        
        message = f'The file "{fn}" was uploaded successfully.'
    
    else:
        message = f'The file "{fn}" already exists.'

else:
    message = 'No file was uploaded'

print ("""\
Content-Type: text/html\n
<html>
<body>
   <p>%s</p>
</body>
</html>
""" % (message,))