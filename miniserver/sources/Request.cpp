/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 17:14:44 by maricard          #+#    #+#             */
/*   Updated: 2023/12/02 15:33:50 by maricard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include "Request.hpp"

Request::Request()
{
}

Request::Request(int connection) :
	_bodyLength(0),
	_maxBodySize(0),
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
			" to " + port.str(), INFORMATION);
}

int	Request::parseRequest(Cluster& cluster, char* buffer, int bytesAlreadyRead)
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
	while (std::strncmp(buffer + pos, "\r\n\r\n", 4) != 0)
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
	    _header["Content-Type"].find("application/octet-stream") == std::string::npos)))
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

int Request::parseBody(char* buffer, int bytesToRead)
{	
	for (int i = 0; i < bytesToRead; i++)
		_body.push_back(buffer[i]);

	if (_body.size() == _bodyLength)
			return 0;

	ssize_t bytesRead;
	char new_buffer[4096];
	while ((bytesRead = recv(_connection, new_buffer, 4096, 0)) > 0)
	{
		for (int i = 0; i < bytesRead; i++)
			_body.push_back(new_buffer[i]);
		
		for (size_t i = 0; i < sizeof(new_buffer); ++i)
			new_buffer[i] = '\0';

		if (_body.size() == _bodyLength)
			return 0;
	}

	if (_body.size() != _bodyLength)
		return 400;
	else if (bytesRead == -1)
		return 500;

	return 0;
}

int Request::parseChunkedRequest(char* buffer, int bytesToRead)
{
	for (int i = 0; i < bytesToRead; i++)
		_body.push_back(buffer[i]);

	ssize_t bytesRead;
	char new_buffer[4096];
	std::vector<char> chunkedBody;
	while ((bytesRead = recv(_connection, new_buffer, 4096, 0)) > 0)
	{
		for (int i = 0; i < bytesRead; i++)
			chunkedBody.push_back(new_buffer[i]);

		for (size_t i = 0; i < sizeof(new_buffer); ++i)
			new_buffer[i] = '\0';
	}

	int chunkCharSize = getHexSize(chunkedBody, 0);
	int chunkSize = getHexFromChunked(chunkedBody, 0);
	int pos = 0;
	while (chunkSize > 0)
	{
		pos += chunkCharSize + 2;
		for (int i = 0; i < chunkSize; i++)
			_body.push_back(chunkedBody[pos + i]);
		
		pos += chunkSize + 2;
		chunkCharSize = getHexSize(chunkedBody, pos);
		chunkSize = getHexFromChunked(chunkedBody, pos);
	}
	
	return 0;
}


static int selectOptionAndReturn(Request& request, Server& server, const Location* location)
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
	if (stat((this->_path).c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
	{
		bool hasFile = false;
		std::vector<std::string> indexes;
		
		if (location && !location->getIndex(server).empty())
			indexes = location->getIndex(server);
		else
			indexes = server.getIndex();
		
		std::vector<std::string>::iterator it = indexes.begin();
		for (; it != indexes.end(); ++it)
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

	std::cout << F_YELLOW "Content-Length: " RESET << _body.size() << std::endl;
	
	/*
	if (!_header.empty())
	{
		std::cout << F_YELLOW "Header: " RESET << std::endl;

		std::map<std::string, std::string>::iterator it = _header.begin();
		for (; it != _header.end(); it++)
			std::cout << it->first + ": " << it->second << std::endl;
	}
	*/

	//if (!_body.empty())
	//{
	//	std::cout << F_YELLOW "Body: " RESET << std::endl;

	//	std::vector<char>::iterator it = _body.begin();
	//	for (; it != _body.end(); it++)
	//		std::cout << *it;
	//}
}
