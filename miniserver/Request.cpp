/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 17:14:44 by maricard          #+#    #+#             */
/*   Updated: 2023/11/06 20:50:44 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include "Request.hpp"
#include "Response.hpp"

Request::Request()
{
}

Request::Request(char* buffer, Server* server) :
	_buffer(buffer),
	_bodyLength(-1),
	_maxBodySize(server->getClientMaxBodySize()),
	_uploadStore("/cgi-bin/upload")
{
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
	_query = other._query;
	_protocol = other._protocol;
	_header = other._header;
	_body = other._body;
	//_argv = other._argv;
	//_envp = other._envp;
	_bodyLength = other._bodyLength;
	_maxBodySize = other._maxBodySize;
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

std::string Request::getQuery() const
{
	return _query;
}

std::string Request::getProtocol() const
{
	return _protocol;
}

static int respondWithError(const std::string& errorCode, std::string& response)
{
	response = Response::buildErrorResponse(errorCode);
	return (-1);
}

int	Request::parseBody(char* body_buffer, int bytesRead, std::string& response)
{
	for (int i = 0; i < bytesRead; i++)
		_body.push_back(body_buffer[i]);

	if (_body.size() > _maxBodySize)
		return respondWithError("413", response);

	return 1;
}

int	Request::parseRequest(char* header_buffer, int bytesRead, std::string& response)
{
	(void)bytesRead;
	std::string request = header_buffer;
	std::stringstream ss(request);
	std::string line;

	ss >> _method >> _path >> _protocol;

	if (_path.find("?") != std::string::npos)
	{
		_query = _path.substr(_path.find("?") + 1, _path.length());
		_path = _path.substr(0, _path.find("?"));
	}

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

	if (line != "\r")
		return respondWithError("413", response);

	if (!checkErrors(response))
		return -1;

	size_t pos = request.find("\r\n\r\n") + 4;
	unsigned int k = 0;

	for(int i = pos; i < bytesRead; i++)
	{
		_body.push_back(header_buffer[i]);
		k++;
	}

	if (k < _bodyLength)
		return _bodyLength - k;

	return 0;
}

//! handle cgi error
std::string	Request::runCGI()
{
	setArgv();
	setEnvp();

	std::string filename = ".tmp";

	{
		std::ofstream file(filename.c_str());
		if (file.is_open())
		{
			for (unsigned i = 0; i < _body.size(); i++)
				file <<  _body[i];
			file.close();
		}
		else
			return Response::buildErrorResponse("500");
	}

    FILE*	file = std::fopen(filename.c_str(), "r");
	int file_fd = -1;

	if (file != NULL)
        file_fd = fileno(file);
	else
		return Response::buildErrorResponse("500");

	int pipe_write[2];
	if (pipe(pipe_write) == -1)
		return Response::buildErrorResponse("500");

	int status;
	int pid = fork();
	if (pid == 0)
	{
		dup2(file_fd, STDIN_FILENO);
		close(file_fd);

    	dup2(pipe_write[WRITE], STDOUT_FILENO);
    	close(pipe_write[WRITE]);
		close(pipe_write[READ]);

		execve(_argv[0], _argv, _envp);
		exit(EXIT_FAILURE);
	}
	else
	{
		close(file_fd);
		close(pipe_write[WRITE]);
		waitpid(pid, &status, 0);

		if (!WIFEXITED(status))
			return Response::buildErrorResponse("500"); 
	}
	
	if (std::remove(filename.c_str()) != 0)
		return Response::buildErrorResponse("500");

	char buffer[4096] = "\0";

	if (read(pipe_write[READ], buffer, 4096) <= 0)
		return Response::buildErrorResponse("500");

	std::string response = buffer;

	close(pipe_write[READ]);
	std::fclose(file);

	deleteMemory();

	std::map<std::string, std::string> header;
	std::vector<std::string> body;
	
	header["HTTP/1.1"] = "202 OK";
	header["Content-Type"] = "text/html";
	body.push_back(response);

	return (Response::buildResponse(header, body));
}

//! remove python hardcode
//! check extensions
void	Request::setArgv()
{
	_argv[0] = myStrdup("/usr/bin/python3");
	_argv[1] = myStrdup(_path.c_str());
	_argv[2] = NULL;
}

void	Request::setEnvp()
{
	int i = 0;

	if (!(_uploadStore.empty()) && i < 17)
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

int	Request::checkErrors(std::string& response)
{
	if (_method == "POST" && (_header["Content-Type"].empty() ||
		_header["Content-Type"].find("multipart/form-data") == std::string::npos))
	{
		return respondWithError("415", response);
	}
	if (_method == "POST" && _header["Content-Length"].empty())
	{
		return respondWithError("411", response);
	}
	else
	{
		std::istringstream ss(_header["Content-Length"]);
		ss >> _bodyLength;
	}

	return 1;
}

void	Request::deleteMemory()
{
	for (unsigned i = 0; i < 3; ++i)
        delete[] _argv[i];

    for (unsigned i = 0; i < 17; ++i)
        delete[] _envp[i];
}

char*	Request::myStrdup(const char* source)
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

static int selectOptionAndReturn(Request& request,
								 Server& server,
								 const Location* location)
{
	if (request.getMethod() == "GET" && location &&
		!location->getCgiPass(server).empty())
		return (GET | CGI);
	else if (request.getMethod() == "GET")
		return (GET);
	if (request.getMethod() == "POST" && location &&
		!location->getCgiPass(server).empty())
		return (POST | CGI);
	if (request.getMethod() == "DELETE")
		return (DELETE);
	return (0);
}

int Request::isValidRequest(Server& server, std::string& response)
{
	if (this->_protocol != "HTTP/1.1")
		return (respondWithError("505", response));
	if (this->_method != "GET" && this->_method != "POST" &&
		this->_method != "DELETE")
		return (respondWithError("501", response));
	if (this->_method == "POST" && this->_body.empty())
		return (respondWithError("204", response));
	/* Check if server has a root path defined, if not return default file */
	if (this->_method == "GET" && server.getRoot().empty())
	{
		this->_path = ""; //TODO path to default index
		return (selectOptionAndReturn(*this, server, NULL));
	}
	else if (server.getRoot().empty())
		return (respondWithError("403", response));
	/* Check if can perform request based on method, within specified location */
	std::string path = this->_path;
	Location* location = server.getLocation(path);
	while (!location && !path.empty())
		location = server.getParentLocation(path);
	if (location)
	{
		if (location->getRoot(server) != server.getRoot())
		{
			this->_path.erase(0, path.length());
			this->_path.insert(0, location->getRoot(server));
		}
		else
			this->_path.insert(0, server.getRoot());
		this->_uploadStore = location->getUploadStore(server);
	}
	else
		this->_path.insert(0, server.getRoot());
	if (location && (!location->isMethodAllowed(this->_method) ||
					 (this->_method == "POST" &&
					  location->getCgiPass(server).empty())))
		return (respondWithError("405", response));
	/*
	 * If is directory, check try to find index
	 * if no index, check if autoindex on
	 * If autoindex off, 403 Forbidden
	 */
	if (*this->_path.end() == '/')
		*this->_path.end() = '\0';

	struct stat sb = {};
	if (stat((this->_path).c_str(), &sb) == 0 &&
		S_ISDIR(sb.st_mode))
	{
		bool hasFile = false;
		std::vector<std::string> indexes;
		if (location && !location->getIndex(server).empty())
			indexes = location->getIndex(server);
		else
			indexes = server.getIndex();
		for (std::vector<std::string>::iterator it = indexes.begin();
			 it != indexes.end(); ++it)
		{
			if (!access((this->_path + "/" + (*it)).c_str(), F_OK))
			{
				hasFile = true;
				this->_path += "/" + (*it);
				break;
			}
		}
		if (!hasFile)
		{
			if (location && location->getAutoindex());// run directory listing
			else
				return (respondWithError("403", response));
		}
	}

	/* Check if file exists and has correct permissions */
	if (access(this->_path.c_str(), F_OK))
		return (respondWithError("404", response));
	if ((this->_method == "POST" && location &&
		 !location->isMethodAllowed(this->_method)) ||
		access(this->_path.c_str(), R_OK))
		return (respondWithError("403", response));
	return (selectOptionAndReturn(*this, server, location));
}

void	Request::displayVars()
{
	std::cout << F_YELLOW "Protocol: " RESET + _protocol << std::endl;
	std::cout << F_YELLOW "Method: " RESET + _method << std::endl;
	std::cout << F_YELLOW "Path: " RESET + _path << std::endl;
	std::cout << F_YELLOW "Query: " RESET + _query << std::endl;

	//if (_header.size() > 0)
	//	std::cout << F_YELLOW "Header: " RESET << std::endl;
	
	//std::map<std::string, std::string>::iterator it = _header.begin();
	//for (; it != _header.end(); it++)
	//	std::cout << it->first + ": " << it->second << std::endl;

	if (_body.size() > 0)
		std::cout << F_YELLOW "Body: " RESET << _body.size() << std::endl;
}
