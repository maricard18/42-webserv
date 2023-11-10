/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 20:15:28 by maricard          #+#    #+#             */
/*   Updated: 2023/11/10 15:02:23 by maricard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"

Cgi::Cgi()
{

}

Cgi::Cgi(Request& request) : _method(request.getMethod()),
							 _path(request.getPath()),
							 _query(request.getQuery()),
						   	 _uploadStore(request.getUploadStore()),
							 _header(request.getHeader()), 
						   	 _body(request.getBody())
{
	for (unsigned i = 0; i < 3; ++i)
        _argv[i] = NULL;

    for (unsigned i = 0; i < 17; ++i)
        _envp[i] = NULL;

}

Cgi::Cgi(const Cgi& copy)
{
	*this = copy;
}

Cgi&	Cgi::operator=(const Cgi& other)
{
	if (this == &other)
		return *this;

	_method = other._method;
	_path = other._path;
	_query = other._query;
	_uploadStore = other._uploadStore;
	_header = other._header;
	_body = other._body;
	return *this;
}

Cgi::~Cgi()
{
	for (unsigned i = 0; i < 3; ++i)
        delete[] _argv[i];

    for (unsigned i = 0; i < 17; ++i)
        delete[] _envp[i];
}

std::string	Cgi::runCGI()
{
	std::string filename = ".tmp";
	int 		file_fd = 0;
	FILE*		file = NULL;

	if (!(file_fd = sendDataToCgi(filename, file)))
		return Response::buildErrorResponse(500);

	int pipe_fd[2];
	if (pipe(pipe_fd) == -1)
		return Response::buildErrorResponse(500);

	setArgv();
	setEnvp();
	int status;
	int pid = fork();

	if (pid == 0)
	{
		dup2(file_fd, STDIN_FILENO);
		close(file_fd);

    	dup2(pipe_fd[WRITE], STDOUT_FILENO);
    	close(pipe_fd[WRITE]);
		close(pipe_fd[READ]);

		execve(_argv[0], _argv, _envp);
		exit(EXIT_FAILURE);
	}
	else
	{
		close(file_fd);
		close(pipe_fd[WRITE]);
		waitpid(pid, &status, 0);

		if (!WIFEXITED(status))
			return Response::buildErrorResponse(500);
	}
	
	if (std::remove(filename.c_str()) != 0)
		return Response::buildErrorResponse(500);

	std::string response = readDataFromCgi(pipe_fd[READ]);

	close(pipe_fd[READ]);
	std::fclose(file);

	return (response);
}

int Cgi::sendDataToCgi(std::string filename, FILE*& file)
{
	{
		std::ofstream tmp_file(filename.c_str());
		if (tmp_file.is_open())
		{
			for (unsigned i = 0; i < _body.size(); i++)
				tmp_file <<  _body[i];
			tmp_file.close();
		}
		else
			return -1;
	}

	int	file_fd = 0;
    file = std::fopen(filename.c_str(), "r");

	if (file != NULL)
        return (file_fd = fileno(file));
	else
		return -1;
}

std::string Cgi::readDataFromCgi(int fd)
{
	char buffer[4096] = "\0";

	if (read(fd, buffer, 4096) <= 0)
		return Response::buildErrorResponse(500);

	std::string response = buffer;

	std::map<std::string, std::string> header;
	std::vector<std::string> body;
	
	header["HTTP/1.1"] = "202 OK";
	header["Content-Type"] = "text/html";
	body.push_back(response);

	return (Response::buildResponse(header, body));
}

void	Cgi::setArgv()
{
	//! access of cgi_pass before assiggn || error of cgi
	_argv[0] = myStrdup("/usr/bin/python3");
	_argv[1] = myStrdup(_path.c_str());
	_argv[2] = NULL;
}

void	Cgi::setEnvp()
{
	int i = 0;

	if (!_path.empty())
	{
		std::string str = "PATH_INFO=" + _path;
		_envp[i++] = myStrdup(str.c_str());
	}
	if (!_uploadStore.empty() && i < 17)
	{
		std::string str = "UPLOAD_STORE=" + _uploadStore;
		_envp[i++] = myStrdup(str.c_str());
	}
	else
	{
		std::string str = "UPLOAD_STORE=uploads";
		_envp[i++] = myStrdup(str.c_str());
	}
	if (!(_method.empty()) && i < 17)
	{
		std::string str = "REQUEST_METHOD=" + _method;
		_envp[i++] = myStrdup(str.c_str());
	}
	if (!(_query.empty()) && i < 17)
	{
		std::string str = "QUERY_STRING=" + _query;
		_envp[i++] = myStrdup(str.c_str());
	}
	if (!(_header["Content-Length"].empty()) && i < 17)
	{
		std::string str = "CONTENT_LENGTH=" + _header["Content-Length"];
		_envp[i++] = myStrdup(str.c_str());
	}
	if (!(_header["Content-Type"].empty()) && i < 17)
	{
		std::string str = "CONTENT_TYPE=" + _header["Content-Type"];
		_envp[i++] = myStrdup(str.c_str());
	}

	for (; i < 17; i++)
		_envp[i] = NULL;
}

char*	Cgi::myStrdup(const char* source)
{
    size_t length = 0;

	for (; source[length]; length++)
		;

    char* duplicate = new char[length + 1];

    for (size_t i = 0; i < length; ++i)
        duplicate[i] = source[i];

    duplicate[length] = '\0';

    return duplicate;
}


