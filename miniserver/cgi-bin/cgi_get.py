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

message = parsed_value

print("Content-Type: text/html\n")
print ("""
<!DOCTYPE html>
<html lang="en">

<head>
	<meta charset="UTF-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">\n
	<title>GET</title>
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
			max-width: 700px;
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

		.date-time {
			font-size: 1.5em;
			font-weight: bold;
			margin-bottom: 20px;
		}

		.get-btn {
			font-size: 20px;
			background-color: #007bff;
			color: white;
			padding: 10px 20px;
			border: none;
			border-radius: 8px;
			cursor: pointer;
			margin-bottom: 20px;
		}

		.home-btn {
			font-size: 15px;
			background-color: #ffaa00;
			color: white;
			padding: 10px 20px;
			border: none;
			border-radius: 8px;
			cursor: pointer;
			margin-top: 10px
		}
	</style>
</head>
<link rel="icon" href="assets/verified.ico" type="image/x-icon">
	   
<body>
    <div class="container">
        <h1>Get Current Date and Time</h1>
        <p>Click the button below to get the current date and time:</p>
        <form action="/getDateTime" method="get">
            <div class="date-time" id="date-time">{message}</div>
            <br>
            <button class="get-btn" onclick="getCurrentDateTime()">Get Date and Time</button>
        </form>
        <br>
        <a href="home.html"><button class="home-btn"><b>Home Page</button></a>
    </div>

    <script>
        function getCurrentDateTime() {
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "/getDateTime", true);
            xhr.onreadystatechange = function () {
                if (xhr.readyState == 4 && xhr.status == 200) {
                    document.getElementById("date-time").textContent = xhr.responseText;
                }
            };
            xhr.send();
        }
    </script>
</body>
</html>
""")