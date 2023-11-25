/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 17:14:44 by maricard          #+#    #+#             */
/*   Updated: 2023/11/25 20:19:24 by maricard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include "Request.hpp"

Request::Request()
{
}

Request::Request(Server* server, int connection) :
	_bodyLength(0),
	_maxBodySize(server->getClientMaxBodySize()),
	_connection(connection)
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

int	Request::parseRequest(char* header_buffer, int64_t& bytesLeftToRead)
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

	std::getline(ss, line, '\r');
	while (std::getline(ss, line, '\r') && line != "\n")
	{
    	size_t pos = line.find(':');

		if (pos != std::string::npos)
		{
			std::string first = line.substr(1, pos - 1);
			std::string second = line.substr(pos + 2, line.length());
			_header[first] = second;
    	}
    }

	if (line != "\n")
		return 413;

	int error;
	if ((error = checkErrors()))
		return error;

	uint32_t pos = 0;
	while (std::strncmp(header_buffer + pos, "\r\n\r\n", 4) != 0)
		pos++;
	pos += 4;

	if (!(_header["Transfer-Encoding"].empty()))
	{
		parseChunkedRequest(header_buffer, pos);
		bytesLeftToRead = 0;
		return 0;
	}
	
	uint32_t k = 0;
	for (uint32_t i = pos; i < bytesLeftToRead; i++)
	{
		_body.push_back(header_buffer[i]);
		k++;
	}

	if (k < _bodyLength)
		bytesLeftToRead = _bodyLength - k;
	else
		bytesLeftToRead = _bodyLength;

	return error; // 0 if no error;
}

int	Request::checkErrors()
{
	if (_method == "POST" && (_header["Content-Type"].empty() ||
		(_header["Content-Type"].find("multipart/form-data") == std::string::npos && 
		_header["Content-Type"].find("application/octet-stream") == std::string::npos)))
	{
		return 415;
	}
	if (_method == "POST" && _header["Content-Length"].empty() && _header["Transfer-Encoding"].empty())
	{
		return 411;
	}
	else if (!_header["Content-Length"].empty() && _header["Transfer-Encoding"].empty())
	{
		std::istringstream ss(_header["Content-Length"]);
		ss >> _bodyLength;
		if (_bodyLength > _maxBodySize)
			return 413;
	}
	return 0;
}

int Request::parseChunkedRequest(char* buffer, uint32_t pos)
{
	std::string request = buffer;
	std::stringstream ss(request.substr(pos, request.length()));
	std::string line;
	int bytesToRead;
	uint32_t k = 0;
	(void)k;
	
	ss >> std::hex >> bytesToRead;
	//std::cout << "hex " << bytesToRead << std::endl;
	//std::cout << "buffer: \'" << buffer[pos + 4 + 9] << "\'" << std::endl;
	//while (bytesToRead != 0)
	//{
	//	k += countDigits(bytesToRead) + 2
	//	int i = pos + k + countDigits(bytesToRead) + 2;
	//	for (uint32_t i = pos; i < bytesToRead; i++)
	//	{
	//		if (buffer[i] != '\r' || buffer[i - 1] == '\r')
	//			_body.push_back(buffer[i]);
	//		k++;
	//	}
	//}
	
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
		return ((error = 400));
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
				((location && location->getAutoindex(server)) ||
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

	if (!_query.empty())
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
