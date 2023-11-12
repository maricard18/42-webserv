/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_get.js                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 17:24:21 by maricard          #+#    #+#             */
/*   Updated: 2023/11/12 12:21:40 by maricard         ###   ########.fr       */
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

const message = formattedDT;

const htmlContent = `
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
			padding: 10px 20px;
			border: none;
			border-radius: 8px;
			cursor: pointer;
		}
	</style>
</head>
<link rel="icon" href="/html_pages/verified.ico" type="image/x-icon">
	   
<body>
    <div class="container">
        <h1>Current Date and Time</h1>
        <p>Press the button to check the current date and time</p>
        <form action="/cgi-bin/cgi_get.py" method="GET">
            <div class="date-time" id="date-time">${message}</div>
            <br>
			<input class="get-btn" type="submit" value="Get Date and Time">
		</form>
		<br>
        <a href="/html_pages/home.html"><button class="home-btn"><b>Home Page</button></a>
    </div>
</body>
</html>
`;

console.log(htmlContent);

