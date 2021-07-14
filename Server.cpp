#include "Server.hpp"

Server::Server() : _PORT(0), _sfd(0)
{
	_sockets.clear();
	ft_memset(&_address, 0, sizeof(_address));
}

Server::Server(Server const &cp){*this = cp;}
Server &Server::operator=(Server const &cp)
{
	if ( this != &cp )
	{
		this->_PORT = cp.get_port();
		this->_sfd = cp.get_sfd();
		this->_sockets = cp.get_sockets();
	}
	return *this;
}

Server::~Server()
{
    close(_sfd);
}
void    Server::setup()
{
	int option = 1;
	if ((_sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			throw ErrorException("cannot create socket");

	// setsockopt to reuse the local address when the server is restarted
	if ((setsockopt(_sfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0))
	{
		perror("setsockopt() failed");
		close(_sfd);
		throw ErrorException("setsockopt failed");
	}

	// fcntl for non-blocking socket
	if ((fcntl(_sfd, F_SETFL, O_NONBLOCK)) < 0)
	{
		perror("fcntl() failed");
		close(_sfd);
		throw ErrorException("fcntl failed");
	}

	// bind gets a unique name for the socket
	// htonl : converts a long integer to a network representation
	// htons : converts a short integer to a network representation
	ft_memset(&_address, 0, sizeof(_address));
	_address.sin_family = AF_INET; 
	_address.sin_addr.s_addr = htonl(INADDR_ANY);
	_address.sin_port = htons(_PORT);
	
	if (bind(_sfd,(struct sockaddr *)&_address,sizeof(_address)) == -1)
	{
		perror("bind() failed");
    	close(_sfd);
		throw ErrorException("bind failed");
	}
}

void    Server::listenConnexion()
{
	// listen allows the server to accept incoming client connections
	if (listen(_sfd, 100) < 0) // backlog a voir
	{
		perror("listen() failed");
    	close(_sfd);
		throw ErrorException("listen failed");
	}
}

int     Server::acceptConnexion(int new_socket)
{
	socklen_t	addrlen = sizeof(sockaddr);
	new_socket = accept(_sfd, (struct sockaddr *)&_address, (socklen_t*)&addrlen);
	_sockets.push_back(new_socket);
	return new_socket;
}

int   	Server::receive(int new_socket)
{
	_request.clear();

	char buffer[RECV_SIZE] = {0};
	int ret = recv(new_socket, buffer, RECV_SIZE - 1, 0);
	if (ret <= 0)
		return ret;
	std::cout << GREEN << "--------REQUEST ---------\n" << buffer << "\n--------------------\n" << ENDCOLOR;
	_request.call(buffer, this->_context);
	return ret;
}

int		Server::respond(int new_socket)
{
	int ret = 0;
	Response _response;
	_response.call(_request);
	std::cout << YELLOW << "------RESPONSE ---------\n" << _response.get_response() << "\n--------------------\n" << ENDCOLOR;
	long size = _response.get_response().size();
	ret = send(new_socket , _response.get_response().c_str() , size, 0);
	return ret;
}

// -------------GETTER-------------------

int     		Server::get_sfd() const {return _sfd;}
sockaddr_in     *Server::get_address(){return &_address;}
Context         *Server::get_context(){return &(this->_context);}
int				Server::get_port()const{return _PORT;}
std::vector<int> Server::get_sockets()const{return _sockets;}

// -------------SETTER-------------------

void 			Server::set_port() {this->_PORT = this->_context.get_port();}

// -------------CONFIGURATION-----------------

int    Server::serverConfig(std::string &scope)
{
	// std::cout << "scope--------------------\n " << scope << std::endl << "scope--------------------\n ";
	std::istringstream sscope(scope);
	std::string str;
	while (getline(sscope, str))
	{
		std::stringstream ss(str);
		std::string key;
		ss >> key;
		if (key == "location") ///recuperer les scopes location
		{
			int leftBraket = 1;
			int rightBraket = 0;
			std::string location = str + "\n";
			while (getline(sscope, str))
			{
				std::size_t found = str.find("{");
				if (found != std::string::npos)
					leftBraket++;
				found = str.find("}");
				if (found != std::string::npos)
					rightBraket++;
				if (leftBraket == rightBraket)
					break;
				location += str + "\n";
			}
			if (leftBraket == rightBraket)
			{
				Context *new_location = new Context();
				new_location->copyAttribut(_context);
				if ((new_location->contextConfig(location)) < 0)
				{
					delete new_location;
					return (-1);
				}
				if (new_location->check_context() < 0)
						return (-1);
				if (new_location->get_locationPath() == "/")
					_context.copyslash(*new_location);
				_context.addLocation(new_location->get_locationPath(), new_location);
			}
			else 
				return (-1);
		}
		else
			this->_context.set_contextAttribut(str);
	}
	std::map<std::string, Context*> tmp = _context.get_locations();
	std::map<std::string, Context*>::iterator it = tmp.find("");
	if ( it != tmp.end())
		_context = *(it->second);
	if (check_contex() < 0)
		return (-1);
	this->set_port();
	// this->_context.printContext();
	return (0);
}

int		Server::check_contex()
{
	if (_context.get_port() == -1)
	{
		std::cerr << "ERROR : please indicate host:port \n";
		return (-1);
	}
	if (_context.get_locations().empty() == true && _context.check_context() < 0)
		return (-1);
	return (0);
}