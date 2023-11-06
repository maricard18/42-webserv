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

print("Content-Type: text/html\n")
print ("""
<!DOCTYPE html>
<html lang="en">

<head>
	<meta charset="UTF-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>POST</title>
	<style>
		body {
			font-family: Arial, sans-serif;
			display: flex;
			justify-content: center;
			align-items: center;
			height: 100vh;
			margin: 0;
		}

		.container {
			text-align: center;
			max-width: 400px;
			width: 100%;
		}

		h1 {
			font-size: 3em;
			margin-bottom: 10px;
		}

		p {
			font-size: 1.2em;
			color: #888;
			margin-bottom: 30px;
		}

		.upload-btn-wrapper {
			position: relative;
			overflow: hidden;
			display: inline-block;
			margin-bottom: 20px;
			cursor: pointer;
		}

		.btn {
			border: 2px solid gray;
			color: gray;
			background-color: white;
			padding: 8px 20px;
			border-radius: 8px;
			font-size: 20px;
			font-weight: bold;
		}

		.upload-btn-wrapper input[type=file] {
			font-size: 100px;
			position: absolute;
			left: 0;
			top: 0;
			opacity: 0;
            cursor: pointer;
		}

		.btn-submit {
			font-size: 15px;
			background-color: #007bff;
			color: white;
			padding: 10px 20px;
			border: none;
			border-radius: 8px;
			cursor: pointer;
		}

		.home-btn {
			font-size: 15px;
			background-color: #ffaa00;
			color: white;
			padding: 10px 20px;
			border: none;
			border-radius: 8px;
			cursor: pointer;
		}
	</style>
</head>
<link rel="icon" href="assets/verified.ico" type="image/x-icon">

<body>
	<div class="container">
		<h1>File Upload</h1>
		<p>Select a file from your device and click "Upload".</p>
		<form id="uploadForm" enctype="multipart/form-data">
			<div class="upload-btn-wrapper">
				<button class="btn">Choose a file</button>
				<input type="file" name="filename" id="fileInput" />
			</div>
			<br>
			<input class="btn-submit" type="submit" value="Upload">
		</form>
		<br>
		<div id="uploadStatus" style="color: {'green' if {status} else 'red'};">{message}</div>
		<br>
		<a href="home.html"><button class="home-btn"><b>Home Page</button></a>
	</div>

	<script>
		document.getElementById("uploadForm").addEventListener("submit", function (event) {
			event.preventDefault(); // Prevent default form submission behavior

			var formData = new FormData();
			var fileInput = document.getElementById("fileInput");
			formData.append("filename", fileInput.files[0]);

			var xhr = new XMLHttpRequest();
			xhr.open("POST", "/upload", true);

			xhr.onload = function () {
				if (xhr.status >= 200 && xhr.status < 300) {
					document.getElementById("uploadStatus").textContent = xhr.responseText;
					document.getElementById("uploadStatus").style.color = 'green';
				} else {
					document.getElementById("uploadStatus").textContent = "Upload failed";
					document.getElementById("uploadStatus").style.color = 'red';
				}
			};

			xhr.onerror = function () {
				document.getElementById("uploadStatus").textContent = "Upload failed";
				document.getElementById("uploadStatus").style.color = 'red';
			};

			xhr.send(formData);
		});
	</script>
<body>
</html>
""")