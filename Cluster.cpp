#include "Cluster.hpp"

Cluster::Cluster(){}

Cluster::Cluster(Cluster const &cp){*this = cp;}

Cluster &Cluster::operator=(Cluster const &cp)
{
	if ( this != &cp )
	{
		this->_servers = cp.get_servers();
	}
	return *this;
}

Cluster::~Cluster()
{
	for(std::vector<Server *>::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		(*it)->get_context()->contextClear();
		delete *it;
	}
	_servers.clear();
	// this->serversClear();
}


void    Cluster::clusterSetup()
{
	memset(_fds, 0 , sizeof(_fds));
	int i = 0;
   	for(std::vector<Server *>::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		(*it)->setup();
		(*it)->listenConnexion();
		_fds[i].fd = (*it)->get_sfd();
		std::cout << "Connexion on port : " << (*it)->get_port() << std::endl;
		_fds[i].events = POLLIN;
		i++;
	}
	_nfds = i;
}

void    Cluster::clusterRun()
{
    int				new_socket = -1;
	int				timeout = (3 * 60 * 1000);
	int				current_size = 0;
	int				end_server = FALSE;
	int				compress_array = FALSE;
	int				close_conn;
	int 			ret = 0;

	while(TRUE)
	{
		std::cout << "Waiting..." << std::endl;
		if ((ret = poll(_fds, _nfds, timeout)) == -1)
		{
			perror("  poll() failed");
			break;
		}
		
		if (ret == 0)
		{
			std::cout << "  poll() timed out.  End program." << std::endl;
			break;
		}
		// One or more descriptors are readable
		current_size = _nfds;
		for (int i = 0; i < current_size; i++)
		{
			// Loop and find fds that returned POLLIN and determine listening or active connection.
			if(_fds[i].revents == 0)
				continue;
			for(std::vector<Server *>::iterator it = _servers.begin(); it != _servers.end(); it++)
			{
				std::vector<int> tmp = (*it)->get_sockets();
				if (_fds[i].fd == (*it)->get_sfd())
				{
					// Listening descriptor is readable.
					// std::cout << "  Listening socket is readable" << std::endl;
					do 
					{
						new_socket = (*it)->acceptConnexion(new_socket);
						if (new_socket < 0)
							break;
						if ((fcntl(new_socket, F_SETFL, O_NONBLOCK)) < 0)
						{
							perror("fcntl() failed");
							close(new_socket);
							break;
						}
						// std::cout << "  New incoming connection - new_socket = " << new_socket << std::endl;
						_fds[_nfds].fd = new_socket;
						_fds[_nfds].events = POLLIN;
						_fds[_nfds].revents = 0;
						_nfds++;
					// Loop back up and accept another incoming connection 
					} while (new_socket != -1);
				}
				// else existing connection must be readable
				else if ((find(tmp.begin(), tmp.end(), _fds[i].fd)) != tmp.end())
				{
					// std::cout << "  Descriptor " << _fds[i].fd << " is readable" << std::endl;
					close_conn = FALSE;
					ret = (*it)->receive(_fds[i].fd);
					if (ret < 0)
					{
						perror("  recv() ");
						close_conn = TRUE;
					}
					else if (ret == 0)
					{
						std::cout << "  Connection closed" << std::endl;
						close_conn = TRUE;
					}
					// std::cout << ret << " bytes received" << std::endl;
					else if (ret > 0)
					{
						ret = (*it)->respond(_fds[i].fd);
						if (ret < 0)
						{
							perror("  send() failed");
							close_conn = TRUE;
							break;
						}
						else if (ret == 0)  
						{
							std::cout << "  Nothing to respond" << std::endl;
							close_conn = TRUE;
							break;
						}
					}
					if (close_conn)
					{
						close(_fds[i].fd); 
						_fds[i].fd = -1;
						compress_array = TRUE;
					}
				}
			}

		}
		if (compress_array)
		{
			compress_array = FALSE;
			for (int i = 0; i < _nfds; i++)
			{
				if (_fds[i].fd == -1)
				{
					for(int j = i; j < _nfds; j++)
					{
						_fds[j].fd = _fds[j+1].fd;
					}
				i--;
				_nfds--;
				}
			}
		}
		if (end_server == TRUE)
			break;
	} // while(TRUE)
	for (int i = 0; i < _nfds; i++)
	{
		if(_fds[i].fd >= 0)
		close(_fds[i].fd);
	}
}

void	Cluster::serversClear()
{
	_servers.clear();
}

// -------------GETTER-------------------
std::vector<Server *>     Cluster::get_servers() const{return _servers;}

// -------------SETTER-------------------

// -------------CONFIGURATION-----------------
int	Cluster::checkFile(std::string path) ///voir si le fichier existe
{
	struct stat		sb;
	if (stat(path.c_str(), &sb) < 0) 
	{
		std::cout << "Config file can't open\n";
		return (0);
	}
	return (1);

}

int    Cluster::clusterConfig(std::string &path)
{

	if (!checkFile(path))
		return -1;
	std::ifstream file;
	file.open(path.c_str());
	std::string str;
	while (getline(file, str))
	{
		if (str == "server {")  ///recuperer les scopes server
		{
			int leftBraket = 1;
			int rightBraket = 0;
			std::string scope = "";
			while (getline(file, str))
			{
				
				std::size_t found = str.find("{");
				if (found != std::string::npos)
					leftBraket++;
				found = str.find("}");
				if (found != std::string::npos)
					rightBraket++;
				if (leftBraket == rightBraket)
					break;
				scope += str + "\n";
			}
			if (leftBraket == rightBraket)
			{
				Server *new_server = new Server();
				if ((new_server->serverConfig(scope)) < 0)
				{
					delete new_server;
					return (-1);
				}
				_servers.push_back(new_server);
			}
			else 
				return (-1);
		}
	}
	// printClusterConfig();
	return (0);
}

void	Cluster::printClusterConfig()
{
	for(std::vector<Server *>::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		std::cout << "****************************\n";
		(*it)->get_context()->printContext();
		std::cout << "****************************\n";
	}
}
