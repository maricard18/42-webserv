/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 17:14:44 by maricard          #+#    #+#             */
/*   Updated: 2023/11/23 15:08:23 by maricard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include "Request.hpp"

Request::Request()
{
}

Request::Request(Server* server) :
	_bodyLength(0),
	_maxBodySize(server->getClientMaxBodySize())
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

std::map<std::string, std::string>	Request::getHeader() const
{
	return _header;
}

std::vector<char>	Request::getBody() const
{
	return _body;
}

std::string Request::getUploadStore() const
{
	return _uploadStore;
}

std::string Request::getExtension()
{
	return getFileExtension(_path);
}

std::string Request::getExecutable() const
{
	return _executable;
}

int	Request::parseRequest(char* header_buffer, int64_t& bytesRead)
{
	std::string request = header_buffer;
	std::stringstream ss(request);
	std::string line;

	ss >> _method >> _path >> _protocol;

	if (_path.find('?') != std::string::npos)
	{
		_query = _path.substr(_path.find('?') + 1, _path.length());
		_path = _path.substr(0, _path.find('?'));
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
		return 413;

	int error;
	if ((error = checkErrors()))
		return error;

	uint32_t pos = 0;
	while (std::strncmp(header_buffer + pos, "\r\n\r\n", 4) != 0)
		pos++;
	pos += 4;

	uint32_t k = 0;
	for (uint32_t i = pos; i < bytesRead; i++)
	{
		_body.push_back(header_buffer[i]);
		k++;
	}

	if (k < _bodyLength)
		bytesRead = _bodyLength - k;

	return error; // 0 if no error;
}

int	Request::checkErrors()
{
	if (_method == "POST" && (_header["Content-Type"].empty() ||
		_header["Content-Type"].find("multipart/form-data") == std::string::npos))
	{
		return 415;
	}
	if (_method == "POST" && _header["Content-Length"].empty())
	{
		return 411;
	}
	else if (!_header["Content-Length"].empty())
	{
		std::istringstream ss(_header["Content-Length"]);
		ss >> _bodyLength;
		if (_bodyLength > _maxBodySize)
			return 413;
	}
	return 0;
}

int	Request::parseBody(char* body_buffer, int64_t bytesRead)
{
	for (int i = 0; i < bytesRead; i++)
		_body.push_back(body_buffer[i]);
	return 0;
}

static int selectOptionAndReturn(Request& request,
								 Server& server,
								 const Location* location)
{
	if (request.getMethod() == "GET" && location && !location->getCgiPass(server).empty())
		return (GET | CGI);
	else if (request.getMethod() == "GET")
		return (GET);
	if (request.getMethod() == "POST")
		return (POST | CGI);
	if (request.getMethod() == "DELETE")
		return (DELETE);
	return (0);
}

int Request::isValidRequest(Server& server, int& error)
{
	if (this->_protocol != "HTTP/1.1")
		return ((error = 505));
	if (this->_method != "GET" && this->_method != "POST" &&
		this->_method != "DELETE")
		return ((error = 501));
	if (this->_method == "POST" && this->_body.empty() && !error)
		return ((error = 204));
	if (server.getRoot().empty())
		return ((error = 403));
	/* Check if can perform request based on method, within specified location */
	std::string path = this->_path;
	Location* location = server.getLocation(path);
	if (location)
	{
		_executable = location->getCgiPass(server);
		if (this->_method == "GET" && location->hasRedirect(server))
			return (REDIR);
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
	if (this->_method != "GET" && (!location ||
								   (!location->isMethodAllowed(this->_method) ||
									(this->_method == "POST" &&
									 location->getCgiPass(server).empty()))))
		return ((error = 405));
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
			if (*(this->_path.end() - 1) == '/' &&
				((location && location->getAutoindex()) ||
				 (!location && server.getAutoindex())))
				return (DIR_LIST);
			else
				return ((error = 403));
		}
	}

	/* Check if file exists and has correct permissions */
	if (access(this->_path.c_str(), F_OK))
		return ((error = 404));
	if ((this->_method == "POST" && location &&
		 !location->isMethodAllowed(this->_method)) ||
		access(this->_path.c_str(), R_OK))
		return ((error = 403));
	if (error)
		return (0);
	return (selectOptionAndReturn(*this, server, location));
}

void	Request::displayVars()
{
	std::cout << F_YELLOW "Protocol: " RESET + _protocol << std::endl;
	std::cout << F_YELLOW "Method: " RESET + _method << std::endl;
	std::cout << F_YELLOW "Path: " RESET + _path << std::endl;
	std::cout << F_YELLOW "Query: " RESET + _query << std::endl;

//	if (!_header.empty())
//	{
//		std::cout << F_YELLOW "Header: " RESET << std::endl;
//
//		std::map<std::string, std::string>::iterator it = _header.begin();
//		for (; it != _header.end(); it++)
//			std::cout << it->first + ": " << it->second << std::endl;
//	}
//
//	if (!_body.empty())
//	{
//		std::cout << F_YELLOW "Body: " RESET << std::endl;
//
//		std::vector<char>::iterator it = _body.begin();
//		for (; it != _body.end(); it++)
//			std::cout << *it;
//	}
}
