#include "Request.hpp"

Request::Request(){}

Request::Request(Request const &cp){*this = cp;}

Request &Request::operator=( Request const & rhs )
{
    if ( this != &rhs )
	{
		this->_strRequest = rhs._strRequest;
		this->_methode = rhs._methode;
		this->_path = rhs._path;
		this->_version = rhs._version;
		this->_headers = rhs._headers;
		this->_body = rhs._body;
		this->_statusCode = rhs._statusCode;
		this->_context = rhs._context;
		this->_uri = rhs._uri;
		this->_query = rhs._query;
		this->_contentType = rhs._contentType;
	}
	return *this;
}

Request::~Request(){}

void Request::clear()
{
	this->_strRequest = "";
	this->_methode = "";
	this->_path = "";
	this->_version = "";
	this->_headers.clear();
	this->_body = "";
	this->_statusCode = 0;
	this->_uri = "";
	this->_query = "";
	this->_contentType = "";
}

void    Request::call(std::string str, Context &context)
{
	_strRequest = str;
	this->pars();
	this->set_context(context, _path);
	pars_url(_path);
}

void	Request::pars()
{
	std::istringstream strRequest(_strRequest);
	std::string str;
	_headers.clear();
	// first line
	std::getline(strRequest, str);
	std::stringstream ss(str);
	ss >> _methode;
	ss >> _path;
	ss >> _version;
	while (getline(strRequest, str) && str != "\r" && str != "")
	{
		std::stringstream ss(str);
		std::string key;
		std::string value;
		ss >> key;
		value.assign(str, key.size(), str.size() - key.size());
		_headers[key] = value;
	}
	while (getline(strRequest, str))
		_body += str + "\n";
}

void    Request::pars_url(std::string path)
{
	size_t pos = path.find("?");
	_uri = path;
	if (pos != std::string::npos)
	{
		_path = path.substr(0, pos);
		_query = path.substr(pos, path.length());
	}
}

// -------------SETTER-------------------

void    Request::set_context(Context &context, std::string path)
{
	std::map<std::string, Context*> tmp = context.get_locations();
	size_t position = path.find_last_of("/");
	std::string extension = "*";
	std::string ret;
	if (position)
	{
		std::string new_path = path;
		while ((ret = ft_getNextDir(&new_path)) != "")
		{
			std::map<std::string, Context*>::iterator it = tmp.find(new_path);
			if (it != tmp.end())
			{
				std::string new_str = path;
				new_str.erase(0, new_path.length());
				this->set_context(*(it->second), new_str);
				break;
			}
		}
	}
	if (!position || ret == "")
		_context = context;
	position = path.find_last_of("."); //trouver l'extension
	tmp = _context.get_locations();
	if (position != std::string::npos)
	{
		extension += path.substr(position);
		std::map<std::string, Context*>::iterator it = tmp.find(extension);
		if ( it != tmp.end())
			_context = *(it->second);
	}
}

void    Request::set_contentType(std::string type){_contentType = type;}

void    Request::set_body(std::string &body){_body = body;}


// -------------GETTER-------------------
std::string Request::get_methode(){return _methode;}
std::string Request::get_path(){return _path;}
std::string Request::get_version(){return _version;}
std::map<std::string, std::string> Request::get_headers(){return _headers;}
std::string Request::get_body(){return _body;}
int         Request::get_statusCode() {return _statusCode;}
Context     Request::get_context(){return _context;}
std::string Request::get_query(){return _query;}
std::string Request::get_uri(){return _uri;}
std::string Request::get_contentType(){return _contentType;}

void	Request::printRequest()
{
	std::cout << "METHOD : " << _methode << std::endl;
    std::cout << "PATH : " << _path << std::endl;
    std::cout << "VESION : " << _version << std::endl;
	for (std::map<std::string,std::string>::iterator it=_headers.begin(); it!=_headers.end(); ++it)
   		std::cout << it->first << " => " << it->second << '\n';
    std::cout << "BODY : " << _body << std::endl;
    std::cout << "STATUS : " <<  _statusCode << std::endl;
	std::cout << "CONTEXT : " << _context.get_locationPath() << std::endl; 
	std::cout << "INDEX : " << _context.get_index() << std::endl; 
}