/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_get.js                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 17:24:21 by maricard          #+#    #+#             */
/*   Updated: 2023/12/02 22:12:47 by maricard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Get current date and time
const dt = new Date();

// Format to dd/mm/YYYY H:M:S
const DateOptions = {
  day: '2-digit',
  month: '2-digit',
  year: 'numeric'
};

const TimeOptions = {
	hour: '2-digit',
	minute: '2-digit',
	second: '2-digit',
	hour12: false
  };

const formattedDT = dt.toLocaleDateString('en-GB', DateOptions) + ' ' + 
					dt.toLocaleTimeString('en-GB', TimeOptions);

message = formattedDT;

if (message === null)
{
	message = "No date and time found";
	header = "HTTP/1.1 500 Internal Server Error\r\n";
}
else
	header = "HTTP/1.1 200 OK\r\n";

const htmlContent = header + `
<!DOCTYPE html>
<html lang="en">

<head>
	<meta charset="UTF-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
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
			max-width: 600px;
			width: 100%;
			background-color: #ffffff;
			padding: 20px;
			border-radius: 10px;
		}

		.button-container {
			display: flex;
			justify-content: space-around;
		}
		
		.form-inline {
			margin: 0 0px; /* Adjust the margin to control spacing between forms */
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
			font-weight: bold;
			background-color: #007bff;
			color: white;
			padding: 10px 20px;
			border: none;
			border-radius: 8px;
			cursor: pointer;
			margin-bottom: 10px;
		}

		.home-btn {
			font-size: 20px;
			background-color: #ffaa00;
			color: white;
			padding: 12px 40px;
			border: none;
			border-radius: 8px;
			cursor: pointer;
			margin-top: 20px;
		}
	</style>
</head>
<link rel="icon" href="/html_pages/verified.ico" type="image/x-icon">

<body>
	<div class="container">
		<h1>Current Date and Time</h1>
		<p>Press the button to check the current date and time</p>

		<div class="date-time" id="date-time-js">${message}</div>

		<div class="button-container">

			<form class="form-inline" action="/cgi-bin/cgi_get.py" method="GET">
				<br>
				<input class="get-btn" type="submit" value="python program">
			</form>

			<form class="form-inline" action="/cgi-bin/cgi_get.js" method="GET">
				<br>
				<input class="get-btn" type="submit" value="javascript program">
			</form>

		</div>

		<br>
		<a href="/html_pages/home.html"><button class="home-btn"><b>Home Page</button></a>
	</div>
</body>
</html>
`;

console.log(htmlContent);

