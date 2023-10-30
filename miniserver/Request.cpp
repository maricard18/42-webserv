/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 17:14:44 by maricard          #+#    #+#             */
/*   Updated: 2023/10/26 15:39:23 by maricard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request()
{	
}

Request::Request(std::string request) : _request(request)
{
	parseRequest(request);
	setArgv();
	setEnvp();
	displayVars();
	
	if(hasCGI() == true)
	{
		MESSAGE("CGI running", WARNING);
		runCGI();
		MESSAGE("CGI finished", WARNING);
	}
}

Request::Request(const Request& copy)
{
	*this = copy;
}

Request::~Request()
{
	_header.clear();
	_body.clear();
}

Request& Request::operator=(const Request& other)
{
	if (this == &other)
		return *this;

	_method = other._method;
	_path = other._path;
	_protocol = other._protocol;
	return *this;
}

std::string Request::getMethod() const
{
	return _method;
}

std::string Request::getPath() const
{
	return _path;
}

std::string Request::getProtocol() const
{
	return _protocol;
}

void	Request::setArgv()
{
	_argv[0] = strdup("/usr/bin/python3");
	_argv[1] = strdup("cgi-bin/cgi_post.py");
	_argv[2] = NULL;
}

void	Request::setEnvp()
{
	int i = 0;

	if (!(_method.empty()))
	{
		std::string string = "REQUEST_METHOD=" + _method;
		_envp[i++] = strdup(string.c_str());
	}
	if (!(_header["Content-Length"].empty()))
	{
		std::string string = "CONTENT_LENGTH=" + _header["Content-Length"];
		_envp[i++] = strdup(string.c_str());
	}
	if (!(_header["Content-Type"].empty()))
	{
		std::string string = "CONTENT_TYPE=" + _header["Content-Type"];
		_envp[i++] = strdup(string.c_str());
	}
	
	_envp[i] = NULL;
}

bool Request::hasCGI()
{
	//! temporary solution
	if (getMethod() == "GET" && getPath() == "/getDateTime")
		return true;
	else if (getMethod() == "POST" && getPath() == "/uploadFile")
		return true;
	return false;
}

void	Request::parseRequest(std::string request)
{
	std::stringstream ss(request);
	std::string line;

	ss >> _method >> _path >> _protocol;

	std::getline(ss, line);
	while (std::getline(ss, line) && line != "\r")
	{
    	size_t pos = line.find(':');
    
		if (pos != std::string::npos)
		{
			std::string first = line.substr(0, pos);
			std::string second = line.substr(pos + 2, line.length());
			_header[first] = second;
    	}
    }

	if (_method == "POST" && _header["Content-Length"].empty())
	{
		//! error 411 Length Required
		MESSAGE("POST request without Content-Length", ERROR);
		return;
	}
	else if (_method == "POST" && (_header["Content-Type"].empty() ||
		_header["Content-Type"].find("multipart/form-data") == std::string::npos))
	{
		//! error 415 Unsupported Media Type
		MESSAGE("POST request without Content-Type", ERROR);
		return;
	}
	
	while (std::getline(ss, line))
	{
		_body.push_back(line);
	}
}

void	Request::runCGI()
{
	int pipe_read[2];
	if (pipe(pipe_read) == -1)
	{
		MESSAGE("pipe error", ERROR);
		return;
	}

	int pipe_write[2];
	if (pipe(pipe_write) == -1)
	{
		MESSAGE("pipe error", ERROR);
		return;
	}

	std::string body;
	for (unsigned i = 0; i < _body.size(); i++)
	{
		body += _body[i];
		
		if (i + 1 < _body.size())
			body += '\n';
	}

	write(pipe_read[WRITE], body.c_str(), 4096);
	close(pipe_read[WRITE]);
	
	int pid = fork();
	if (pid == 0)
	{
		// child process
		dup2(pipe_read[READ], STDIN_FILENO);
		close(pipe_read[READ]);

    	dup2(pipe_write[WRITE], STDOUT_FILENO);
    	//close(pipe_write[WRITE]);
		
		close(pipe_read[WRITE]);
		close(pipe_write[READ]);

		execve(_argv[0], _argv, _envp);
		MESSAGE("execve error", ERROR);
		exit(0);
	}
	else
	{
		// parent process
		close(pipe_read[READ]);
		close(pipe_write[WRITE]);
		waitpid(pid, NULL, 0);
	}

	char buffer[4096];

	read(pipe_write[READ], buffer, 4096);

	std::cout << std::endl
			  << F_RED "OUTPUT: " RESET 
			  << std::endl 
			  << buffer 
			  << std::endl;

	close(pipe_write[READ]);
}

void	Request::displayVars()
{
	std::cout << F_RED "REQUEST VALUES" RESET << std::endl;
	std::cout << F_YELLOW "Method: " RESET << std::endl << _method << std::endl;
	std::cout << F_YELLOW "Path: " RESET << std::endl << _path << std::endl;
	std::cout << F_YELLOW "Protocol: " RESET << std::endl << _protocol << std::endl;

	if (_header.size() > 0)
		std::cout << F_YELLOW "header:" RESET << std::endl;
	
	std::map<std::string, std::string>::iterator it = _header.begin();
	for (; it != _header.end(); it++)
		std::cout << it->first + ": " << it->second << std::endl;

	if (_body.size() > 0)
		std::cout << F_YELLOW "body:" RESET << std::endl;

	for (unsigned i = 0; i < _body.size(); i++)
		std::cout << _body[i] << std::endl;

	std::cout << F_YELLOW "Argv: " RESET << std::endl;
	for (int i = 0; _argv[i]; i++)
		std::cout << _argv[i] << std::endl;

	std::cout << F_YELLOW "Envp: " RESET << std::endl;
	for (int i = 0; _envp[i]; i++)
		std::cout << _envp[i] << std::endl;
}