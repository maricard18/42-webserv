import cgi, os
import cgitb; cgitb.enable()

name = os.environ.get('POST_DATA', '')

print(name)