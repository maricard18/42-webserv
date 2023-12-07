/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 17:14:44 by maricard          #+#    #+#             */
/*   Updated: 2023/12/04 19:19:00 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include "Request.hpp"

Request::Request()
	: _bodyLength(), _maxBodySize(), _connection(), _server()
{
}

Request::Request(int connection)
	: _bodyLength(0),
	  _maxBodySize(0),
	  _connection(connection),
	  _server()
{
}

Request::Request(const Request& copy)
	: _bodyLength(), _maxBodySize(), _connection(), _server()
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
	_bodyLength = other._bodyLength;
	_maxBodySize = other._maxBodySize;
	_uploadStore = other._uploadStore;
	_connection = other._connection;
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

std::string Request::getHeaderField(const std::string& field)
{
	return _header[field];
}

Server* Request::getServer() const
{
	return (this->_server);
}

void Request::setServer(Server* server)
{
	this->_server = server;
	this->_maxBodySize = server->getClientMaxBodySize();

	Response::setResponseServer(server);
}

static std::string in_addr_t_to_ip(in_addr_t addr)
{
	std::ostringstream oss;

	for (int i = 0; i < 4; ++i)
	{
		in_addr_t octet = (addr >> (i * 8)) & 0xFF;
		oss << octet;

		if (i < 3)
		{
			oss << ".";
		}
	}
	return oss.str();
}

static void selectServer(Cluster& cluster, Request& request)
{
	std::stringstream host(request.getHeaderField("Host"));
	std::string serverName;
	std::vector<Server*> serverList = cluster.getServerList();
	std::vector<Server*>::iterator hostIt = serverList.begin();

	std::getline(host, serverName, ':');
	for (; hostIt != serverList.end(); ++hostIt)
	{
		if ((*hostIt)->isServerName(serverName))
		{
			request.setServer(*hostIt);
			break ;
		}
	}

	struct sockaddr_in clientAddress = {};
	Server* server = request.getServer();
	std::stringstream	port;

	port << server->getListenPort();
	MESSAGE("Connected " +
			in_addr_t_to_ip(clientAddress.sin_addr.s_addr) +
			" to " + port.str(), INFORMATION)
}

int	Request::parseRequest(Cluster& cluster, char* buffer, int64_t bytesAlreadyRead)
{
	int error;
	std::string request = buffer;
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

	selectServer(cluster, *this);

	if ((error = checkErrors()))
		return error;

	uint32_t pos = 0;
	while (request.compare(pos, 4, "\r\n\r\n") != 0)
		pos++;
	pos += 4;

	if (_header["Transfer-Encoding"] == "chunked")
		error = parseChunkedRequest(buffer + pos, bytesAlreadyRead - pos);
	else if (_method == "POST")
		error = parseBody(buffer + pos, bytesAlreadyRead - pos);

	return error;
}

int	Request::checkErrors()
{
	if (_method == "POST" && (_header["Content-Type"].empty() ||
	   (_header["Content-Type"].find("multipart/form-data") == std::string::npos && 
		_header["Content-Type"].find("text/plain") == std::string::npos &&
		_header["Content-Type"].find("image/jpeg") == std::string::npos &&
		_header["Content-Type"].find("image/png") == std::string::npos &&
		_header["Content-Type"].find("application/pdf") == std::string::npos &&
		_header["Content-Type"].find("application/json") == std::string::npos &&
	    _header["Content-Type"].find("application/octet-stream") == std::string::npos &&
		_header["Content-Type"].find("video/mp4") == std::string::npos &&
		_header["Content-Type"].find("audio/mpeg") == std::string::npos )))
	{
		return 415;
	}
	
	if (_method == "POST" && _header["Content-Length"].empty() && _header["Transfer-Encoding"] != "chunked")
	{
		return 411;
	}
	else if (!_header["Content-Length"].empty() && _header["Transfer-Encoding"] != "chunked")
	{
		std::istringstream ss(_header["Content-Length"]);
		ss >> _bodyLength;
		
		if (_bodyLength > _maxBodySize)
			return 413;
	}
	
	return 0;
}

int Request::parseBody(char* previousBuffer, int64_t bytesToRead)
{
	int64_t bytesRead;
	char 	buffer[4096];
	
	for (int i = 0; i < bytesToRead; i++)
		_body.push_back(previousBuffer[i]);

	if (_body.size() == _bodyLength)
		return 0;

	for (unsigned i = 0; i < sizeof(buffer); ++i)
		buffer[i] = '\0';

	while ((bytesRead = recv(_connection, buffer, 4096, 0)) > 0)
	{
		for (unsigned i = 0; i < bytesRead; i++)
			_body.push_back(buffer[i]);
		
		for (unsigned i = 0; i < sizeof(buffer); ++i)
			buffer[i] = '\0';

		if (_body.size() == _bodyLength)
			return 0;
	}

	if (_body.size() != _bodyLength)
		return 400;
	else if (bytesRead == -1)
		return 500;

	return 0;
}

int Request::parseChunkedRequest(char* previousBuffer, int64_t bytesToRead)
{
	int64_t bytesRead;
	char 	buffer[4096];
	std::vector<char> chunkedBody;

	for (unsigned i = 0; i < bytesToRead; i++)
		chunkedBody.push_back(previousBuffer[i]);

	for (unsigned i = 0; i < sizeof(buffer); ++i)
		buffer[i] = '\0';
	
	while ((bytesRead = recv(_connection, buffer, 4096, 0)) > 0)
	{
		for (unsigned i = 0; i < bytesRead; i++)
			chunkedBody.push_back(buffer[i]);

		for (unsigned i = 0; i < bytesRead; ++i)
			buffer[i] = '\0';
	}

	uint32_t chunkCharSize = getHexSize(chunkedBody, 0);
	uint32_t chunkSize = getHexFromChunked(chunkedBody, 0);
	uint32_t pos = chunkCharSize + 2;
	while (chunkSize > 0 && pos < chunkedBody.size())
	{
		for (uint32_t i = 0; i < chunkSize; i++)
			_body.push_back(chunkedBody[pos + i]);
		pos += chunkSize + 2;
		chunkCharSize = getHexSize(chunkedBody, pos);
		chunkSize = getHexFromChunked(chunkedBody, pos);
		pos += chunkCharSize + 2;
	}

	if (chunkSize != 0)
		return 400;
	
	return 0;
}


static int selectOptionAndReturn(Request& request)
{
	if (request.getMethod() == "GET" && !request.getExecutable().empty())
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
		if (location->getRoot(server) != server.getRoot())
		{
			std::string root = location->getRoot(server);
			Location* rootFileLocation = server.getLocation(root);
			if (rootFileLocation && rootFileLocation->getPath().at(0) == '.')
				_executable = server.getLocation(root)->getCgiPass(server);
		}
		if (_executable.empty())
			_executable = location->getCgiPass(server);
		if (location->getPath().at(0) == '.')
			location = server.getParentLocation(path);
		if (location && this->_method == "GET" && location->hasRedirect(server))
			return (REDIR);
		if (location && location->getRoot(server) != server.getRoot())
		{
			this->_path.erase(0, path.length());
			this->_path.insert(0, location->getRoot(server));
		}
		else
			this->_path.insert(0, server.getRoot());
		if (location)
			this->_uploadStore = location->getUploadStore(server);
	}
	else
		this->_path.insert(0, server.getRoot());

	/*
	 * If is directory, check try to find index
	 * if no index, check if autoindex on
	 * If autoindex off, 403 Forbidden
	 */
	struct stat sb = {};
	if (stat((this->_path).c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
	{
		std::vector<std::string> indexes;
		
		if (location && !location->getIndex(server).empty())
			indexes = location->getIndex(server);
		else
			indexes = server.getIndex();
		
		std::vector<std::string>::iterator it = indexes.begin();
		for (; it != indexes.end(); ++it)
		{
			if (!access((this->_path + (*it)).c_str(), F_OK))
			{
				this->_path += (*it);
				path = this->_path;
				Location* fileLocation = server.getLocation(path);
				if (fileLocation)
					_executable = fileLocation->getCgiPass(server);
				goto validateRequest;
			}
		}
		if (*(this->_path.end() - 1) == '/' &&
		   ((location && location->getAutoindex(server)) ||
		   (!location && server.getAutoindex())))
			return (DIR_LIST);
		else
			return ((error = 403));
	}
validateRequest:
	if ((!location && this->_method != "GET") || (location &&
	(!location->isMethodAllowed(this->_method) ||
	(this->_method == "POST" && _executable.empty()))))
		return ((error = 405));

	/* Check if file exists and has correct permissions */
	if (access(this->_path.c_str(), F_OK))
		return ((error = 404));
	if ((this->_method == "POST" && location && 
	    !location->isMethodAllowed(this->_method)) ||
		access(this->_path.c_str(), R_OK))
		return ((error = 403));
	if (error)
		return (0);
	return (selectOptionAndReturn(*this));
}

void	Request::displayVars()
{
	MESSAGE(_protocol + " " + _method + " " + _path, REQUEST)
	
//	std::cout << F_YELLOW "Protocol: " RESET + _protocol << std::endl;
//	std::cout << F_YELLOW "Method: " RESET + _method << std::endl;
//	std::cout << F_YELLOW "Path: " RESET + _path << std::endl;
//
//	if (!_query.empty())
//		std::cout << F_YELLOW "Query: " RESET + _query << std::endl;
//
//	std::cout << F_YELLOW "Content-Length: " RESET << _body.size() << std::endl;
//
//
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
