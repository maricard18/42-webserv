/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 19:09:05 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/11/09 20:29:56 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

std::map<std::string, std::string> Response::_errorStatus;
std::map<std::string, std::string> Response::_redirStatus;

Response::Response()
{
	initializeErrorStatus();
}

Response::Response(const Response&)
{
}

Response& Response::operator=(const Response&)
{
	return (*this);
}

Response::~Response()
{
}

void Response::initializeErrorStatus()
{
	if (!_errorStatus.empty())
		return;
	Response::_errorStatus["400"] = "Bad Request";
	Response::_errorStatus["401"] = "Unauthorized";
	Response::_errorStatus["402"] = "Payment Required";
	Response::_errorStatus["403"] = "Forbidden";
	Response::_errorStatus["404"] = "Not Found";
	Response::_errorStatus["405"] = "Method Not Allowed";
	Response::_errorStatus["406"] = "Not Acceptable";
	Response::_errorStatus["407"] = "Proxy Authentication Required";
	Response::_errorStatus["408"] = "Request Timeout";
	Response::_errorStatus["409"] = "Conflict";
	Response::_errorStatus["410"] = "Gone";
	Response::_errorStatus["411"] = "Length Required";
	Response::_errorStatus["412"] = "Precondition Failed";
	Response::_errorStatus["413"] = "Request Entity Too Large";
	Response::_errorStatus["414"] = "Request-URI ";
	Response::_errorStatus["415"] = "Unsupported Media Type";
	Response::_errorStatus["416"] = "Range Not Satisfiable";
	Response::_errorStatus["417"] = "Expectation Failed";
	Response::_errorStatus["418"] = "I'm a teapot";
	Response::_errorStatus["421"] = "Misdirected Request";
	Response::_errorStatus["422"] = "Unprocessable Entity";
	Response::_errorStatus["423"] = "Locked";
	Response::_errorStatus["424"] = "Failed Dependency";
	Response::_errorStatus["425"] = "Too Early";
	Response::_errorStatus["426"] = "Upgrade Required";
	Response::_errorStatus["428"] = "Precondition Required";
	Response::_errorStatus["429"] = "Too Many Requests";
	Response::_errorStatus["431"] = "Request Header Fields Too Large";
	Response::_errorStatus["451"] = "Unavailable For Legal Reasons";
	Response::_errorStatus["500"] = "Internal Server Error";
	Response::_errorStatus["501"] = "Not Implemented";
	Response::_errorStatus["502"] = "Bad Gateway";
	Response::_errorStatus["503"] = "Service Unavailable";
	Response::_errorStatus["504"] = "Gateway Timeout";
	Response::_errorStatus["505"] = "HTTP Version Not Supported";
	Response::_errorStatus["506"] = "Variant Also Negotiates";
	Response::_errorStatus["507"] = "Insufficient Storage";
	Response::_errorStatus["508"] = "Loop Detected";
	Response::_errorStatus["510"] = "Not Extended";
	Response::_errorStatus["511"] = "Network Authentication Required";
}

void Response::initializeRedirStatus()
{
	if (!_redirStatus.empty())
		return;
	Response::_redirStatus["300"] = "Multiple Choices";
	Response::_redirStatus["301"] = "Moved Permanently";
	Response::_redirStatus["302"] = "Found";
	Response::_redirStatus["303"] = "See Other";
	Response::_redirStatus["304"] = "Not Modified";
	Response::_redirStatus["305"] = "Use Proxy";
	Response::_redirStatus["307"] = "Temporary Redirect";
}

std::string Response::buildResponse(std::map<std::string, std::string>& header,
									std::vector<std::string>& body)
{
	std::string response;

	response.append("HTTP/1.1 " + header["HTTP/1.1"] + "\n");
	for (std::map<std::string, std::string>::iterator it = header.begin();
		 it != header.end(); ++it)
	{
		if ((*it).first != "HTTP/1.1")
			response.append((*it).first + ": " + (*it).second + "\n");
	}
	response.append("Server: Webserv (Unix)\n");
	response.append(CRLF);
	if (!body.empty())
	{
		for (std::vector<std::string>::iterator it = body.begin();
			 it != body.end(); ++it)
		{
			response.append(*it + "\n");
		}
		response.append(CRLF);
	}
	return (response);
}

std::string Response::buildErrorResponse(int _errorCode)
{
	initializeErrorStatus();

	std::string response;
	std::stringstream errorCode;
	errorCode << _errorCode;
	MESSAGE(errorCode.str() + " " + _errorStatus[errorCode.str()], WARNING);
	if (_errorStatus[errorCode.str()].empty())
	{
		response.append("HTTP/1.1 500 Internal Server Error\n");
		response.append("Content-Type: text/html\n");
		response.append("Server: Webserv (Unix)\n");
		response.append(CRLF);
		response.append("<h1>Internal Server Error</h1>");
		response.append(CRLF);
		return (response);
	}
	response.append(
		"HTTP/1.1 " + errorCode.str() + " " + _errorStatus[errorCode.str()] +
		"\n");
	response.append("Content-Type: text/html\n");
	response.append("Server: Webserv (Unix)\n");
	response.append(CRLF);
	response.append("<!DOCTYPE html>\n"
					"<html lang=\"en\">\n"
					"\n"
					"<head>\n"
					"    <meta charset=\"UTF-8\">\n"
					"    <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n"
					"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
					"    <title>" + errorCode.str() + " " +
					_errorStatus[errorCode.str()] +
					"</title>\n"
					"    <style>\n"
					"        body {\n"
					"            font-family: Arial, sans-serif;\n"
					"            display: flex;\n"
					"            justify-content: center;\n"
					"            align-items: center;\n"
					"            height: 100vh;\n"
					"            margin: 0;\n"
					"        }\n"
					"\n"
					"        .container {\n"
					"            text-align: center;\n"
					"        }\n"
					"\n"
					"        h1 {\n"
					"            font-size: 3em;\n"
					"            margin-bottom: 10px;\n"
					"        }\n"
					"\n"
					"        p {\n"
					"            font-size: 1.2em;\n"
					"            color: #888;\n"
					"        }\n"
					"    </style>\n"
					"</head>\n"
					"<body>\n"
					"    <div class=\"container\">\n"
					"        <h1>" + errorCode.str() + " " +
					_errorStatus[errorCode.str()] + "</h1>\n"
					"        <p>The server has been deserted for a while.<br>Please be patient or try again later.</p>\n"
					"    </div>\n"
					"</body>\n"
					"</html>");
	response.append(CRLF);
	return (response);
}

std::string Response::buildRedirectResponse(const std::pair<std::string,
															std::string>& redirect)
{
	initializeRedirStatus();
	if (_redirStatus.empty())
		return (Response::buildErrorResponse(500));

	std::string response;
	response.append(
		"HTTP/1.1 " + redirect.first + " " + _redirStatus[redirect.first] +
		"\n");
	response.append("Location: " + redirect.second + "\n");
	response.append(CRLF);
	return (response);
}
