#include "Context.hpp"

Context::Context()
{
		// this->_host = "";
		this->_port = -1;
		this->_server_name = "";
		this->_maxBodySize = "";
		this->_root = "";
		this->_autoindex = false;
		this->_index = "";
		this->_cgi = "";
		this->_cgi_pass = std::string();
		this->_upload_pass = "";
		this->_error_page = "";
		this->_locationPath = "";
}
Context::Context(Context const &cp){*this = cp;}
Context &Context::operator=(Context const &cp)
{
	if ( this != &cp )
	{
		// this->_host = cp.get_host();
		this->_port = cp.get_port();
		this->_server_name = cp.get_server_name();
		this->_maxBodySize = cp.get_maxBodySize();
		this->_root = cp.get_root();
		this->_autoindex = cp.get_autoindex();
		this->_index = cp.get_index();
		this->_cgi = cp.get_cgi();
		this->_cgi_pass = cp.get_cgi_pass();
		this->_upload_pass = cp.get_upload_pass();
		this->_error_page = cp.get_error_page();
		this->_locations = cp.get_locations();
		this->_httpRedirection = cp.get_httpRedirection();
		this->_locationPath = cp.get_locationPath();
		this->_methodes = cp.get_methodes();
	}
	return *this;
}
Context::~Context()
{
}

void Context::copyslash(Context const &cp)
{
		// this->_host = cp.get_host();
		this->_port = cp.get_port();
		this->_server_name = cp.get_server_name();
		this->_maxBodySize = cp.get_maxBodySize();
		this->_root = cp.get_root();
		this->_autoindex = cp.get_autoindex();
		this->_index = cp.get_index();
		this->_cgi = cp.get_cgi();
		this->_cgi_pass = cp.get_cgi_pass();
		this->_upload_pass = cp.get_upload_pass();
		this->_error_page = cp.get_error_page();
		this->_httpRedirection = cp.get_httpRedirection();
		this->_methodes = cp.get_methodes();
}

void                        Context::copyAttribut(Context &cp)
{
    // this->_host = cp.get_host();
	this->_port = cp.get_port();
	this->_server_name = cp.get_server_name();
	this->_maxBodySize = cp.get_maxBodySize();
	this->_root = cp.get_root();
	this->_autoindex = cp.get_autoindex();
	this->_index = cp.get_index();
    this->_cgi = cp.get_cgi();
    this->_cgi_pass = cp.get_cgi_pass();
    this->_upload_pass = cp.get_upload_pass();
	this->_error_page = cp.get_error_page();
	this->_httpRedirection = cp.get_httpRedirection();
}

int						Context::contextConfig(std::string scope)
{
    //std::cout << "scope--------------------\n " << scope << std::endl << "scope--------------------\n ";
	std::istringstream sscope(scope);
	std::string str, tmp;
	std::getline(sscope, str);
	std::stringstream ss(str);
	ss >> tmp;
	ss >> this->_locationPath;
	while (getline(sscope, str))
	{
			std::stringstream ss(str);
			std::string key;
			ss >> key;
			if (key == "location")  ////recuperer les scope location
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
					new_location->copyAttribut(*this);
					if ((new_location->contextConfig(location)) < 0)
					{
						delete new_location;
						return (-1);
					}
					if (new_location->check_context() < 0)
						return (-1);
					this->addLocation(new_location->get_locationPath(), new_location);
				}
				else 
					return (-1);
			}
			else
		this->set_contextAttribut(str);	
	}
	// this->printContext();
	return (0);
}

int		Context::check_context()
{
	if (this->_root == "")
	{
		std::cerr << "ERROR : please indicate a root directory for " << _locationPath << std::endl;
		return (-1);
	}
	if (this->_error_page == "" )
	{
		std::cerr << "ERROR : please provide error pages \n";
		return (-1);
	}
	return (0);
}

void	Context::set_contextAttribut(std::string &str)
{
	std::stringstream ss(str);
	std::string key;
	ss >> key;

	typedef void (Context::*funct)(std::string &target);
	funct tab[] = {&Context::set_server_name, &Context::set_listen, &Context::set_erro_page, &Context::set_maxBodySize,
					&Context::set_methode, &Context::set_root, &Context::set_autoindex, &Context::set_index,
					&Context::set_cgi, &Context::set_cgi_pass, &Context::set_upload_pass, &Context::set_httpRedirection};
	std::string attribute[]= {"server_name", "listen", "error_page", "client_max_body_size", "allow_methods", "root",
								"autoindex", "index", "cgi", "cgi_pass", "upload_pass", "return"};
	for (int i = 0; i < 12; i++)
	{
		if (key.compare(attribute[i]) == 0)
		{
			try
			{
				(this->*tab[i])(str);
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << '\n';
			}
		}
	}
	(void)tab;
}
bool						Context::checkMethode(std::string methode){return (this->_methodes.find(methode)->second);}
void						Context::addLocation(std::string path, Context *location)
{
    _locations.insert(std::pair<std::string, Context *>(path, location));
}

void	Context::contextClear()
{
	for(std::map<std::string, Context*>::iterator it = _locations.begin(); it != _locations.end(); it++)
		delete it->second;
	this->_locations.clear();
}


// -------------GETTER-------------------
// std::string						Context::get_host() const{ return this->_host;}
int								Context::get_port()const{return this->_port;}
std::string						Context::get_server_name() const{return this->_server_name;}
std::string						Context::get_maxBodySize() const{return this->_maxBodySize;}
std::string						Context::get_error_page() const{return this->_error_page;}
std::map<std::string, bool>		Context::get_methodes() const{return this->_methodes;}
std::string						Context::get_root() const{return this->_root;}
bool							Context::get_autoindex() const{return this->_autoindex;}
std::string						Context::get_index() const{return this->_index;}
std::string						Context::get_cgi() const{return this->_cgi;}
std::string						Context::get_cgi_pass() const{return this->_cgi_pass;}
std::string						Context::get_upload_pass() const{return this->_upload_pass;}
std::pair<int, std::string>		Context::get_httpRedirection() const{return this->_httpRedirection;}
std::map<std::string, Context *>Context::get_locations() const{return this->_locations;}
std::string						Context::get_locationPath() const{return this->_locationPath;}

// -------------SETTER-------------------
void	Context::set_listen(std::string &str)
{
	std::stringstream ss(str);
	std::string tmp, port;
	ss >> tmp;
	ss >> port;
	this->_port = ft_atoi(port.c_str());
	// std::cout << RED << "port : " << _port << ENDCOLOR << std::endl;
} 
void	Context::set_server_name(std::string &str)
{
	std::stringstream ss(str);
	std::string tmp;
	ss >> tmp;
	ss >> this->_server_name;
}
void	Context::set_maxBodySize(std::string &str)
{
	std::stringstream ss(str);
	std::string tmp;
	ss >> tmp;
	ss >> this->_maxBodySize;

}
void	Context::set_erro_page(std::string &str)
{
	std::stringstream ss(str);
	std::string tmp;
	ss >> tmp;
	ss >> this->_error_page;
}
void	Context::set_methode(std::string &str)
{
	std::stringstream ss(str);
	std::string tmp, methode1, methode2, methode3;
	ss >> tmp;
	ss >> methode1;
	ss >> methode2;
	ss >> methode3;
	if (methode1.compare("GET") == 0 || methode2.compare("GET") == 0 || methode3.compare("GET") == 0)
		_methodes.insert(std::pair<std::string, bool>("GET", true));
	else 
		_methodes.insert(std::pair<std::string, bool>("GET", false));
	if (methode1.compare("POST") == 0 || methode2.compare("POST") == 0 || methode3.compare("POST") == 0)
		_methodes.insert(std::pair<std::string, bool>("POST", true));
	else 
		_methodes.insert(std::pair<std::string, bool>("POST", false));
	if (methode1.compare("DELETE") == 0 || methode2.compare("DELETE") == 0 || methode3.compare("DELETE") == 0)
		_methodes.insert(std::pair<std::string, bool>("DELETE", true));
	else 
		_methodes.insert(std::pair<std::string, bool>("DELETE", false));
}
void	Context::set_root(std::string &str)
{
	std::stringstream ss(str);
	std::string tmp;
	ss >> tmp;
	ss >> this->_root;
}
void	Context::set_autoindex(std::string &str)
{
	std::stringstream ss(str);
	std::string tmp, state;
	ss >> tmp;
	ss >> state;
	if (state.compare("on") == 0)
		this->_autoindex = true;
	else
		this->_autoindex = false;
}
void	Context::set_index(std::string &str)
{
	std::stringstream ss(str);
	std::string tmp;
	ss >> tmp;
	ss >> this->_index;
}
void	Context::set_cgi(std::string &str)
{
	std::stringstream ss(str);
	std::string tmp;
	ss >> tmp;
	ss >> this->_cgi;
}
void	Context::set_cgi_pass(std::string &str)
{
	std::stringstream ss(str);
	std::string tmp;
	ss >> tmp;
	ss >> this->_cgi_pass;
}
void	Context::set_upload_pass(std::string &str)
{
	std::stringstream ss(str);
	std::string tmp;
	ss >> tmp;
	ss >> this->_upload_pass;
}
void	Context::set_httpRedirection(std::string &str)
{
	std::stringstream ss(str);
	std::string tmp, code;
	ss >> tmp;
	ss >> code;
	ss >> this->_httpRedirection.second;
	this->_httpRedirection.first = ft_atoi(code.c_str());
}
void	Context::set_locationPath(std::string &str)
{
	std::stringstream ss(str);
	std::string tmp;
	ss >> tmp;
	ss >> this->_locationPath;
}

void	Context::printContext()
{
	std::cout << "-------------------------context : " << this->_locationPath << "-------------------------\n";
	std::cout << "PORT = " << _port << std::endl;
	std::cout << "NAME = " << _server_name << std::endl;
	std::cout << "BODY SIZE = " << _maxBodySize << std::endl;
	std::cout << "ROOT = " << _root << std::endl;
	std::cout << "AUTOINDEX = " << _autoindex << std::endl;
	std::cout << "INDEX = " << _index << std::endl;
	std::cout << "CGI = " << _cgi << std::endl;
	std::cout << "CGI_PASS = " << _cgi_pass << std::endl;
	std::cout << "UPLOAD = " << _upload_pass << std::endl;
	std::cout << "LOCATION = " << _locationPath << std::endl;
	std::cout << "ERROR = " << _error_page << std::endl;
	std::cout << "REDIRECTION = " << _httpRedirection.first << "-" << _httpRedirection.second << std::endl;
	for(std::map<std::string, bool>::iterator it = _methodes.begin(); it != _methodes.end(); it++)
		std::cout << "METHODE = " << it->first << "-" << it->second << std::endl;
	for(std::map<std::string, Context *>::reverse_iterator it = _locations.rbegin(); it != _locations.rend(); it++)
	{
		std::cout << "LOCATION of " << this->_locationPath << " = " << it->first << std::endl;
		it->second->printContext();
	}
}