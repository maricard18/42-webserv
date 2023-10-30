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

form = cgi.FieldStorage()
print("Form: ", form)

# Get filename here.
fileitem = form['filename']

## Test if the file was uploaded
if fileitem.filename:
   
   # strip leading path from file name to avoid directory traversal attacks
   fn = os.path.basename(fileitem.filename)
   open('./uploads' + fn, 'wb').write(fileitem.file.read())

   message = 'The file "' + fn + '" was uploaded successfully'

else:
   message = 'No file was uploaded'

#print ("""\
#Content-Type: text/html\n
#<html>
#<body>
#   <p>%s</p>
#</body>
#</html>
#""" % (message,))