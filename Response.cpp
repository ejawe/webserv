#include "Response.hpp"


Response::Response() : _statusCode("200"), _displayAutoindex(false)
{
	_path = std::string();

}

Response::Response(Response const &cp){*this = cp;}
Response &Response::operator=(Response const &cp)
{
	if ( this != &cp )
	{
		this->_strResponse = cp.get_response();
		this->_request = cp.get_request();
		this->_path = cp.get_path();
		this->_httpVersion = cp.get_httpVersion();
		this->_statusCode = cp.get_statusCode();
		this->_reasonPhrase = cp.get_reasonPhrase();
		this->_contentType = cp.get_contentType();
		this->_contentLength = cp.get_contentLength();
		this->_body = cp.get_body();
		this->_errors = cp.get_errors();
		this->_types = cp.get_types();
		this->_headers = cp.get_headers();
		this->_displayAutoindex = cp.get_displayAutoindex();
		this->_context = cp.get_context();
	}
	return *this;
}

Response::~Response(){
}

void	Response::initErrorMap()
{
	_errors[200] ="OK"; ///
	_errors[201] ="Created";
	_errors[202] ="Accepted";  // pour upload
	_errors[204] ="No Content";  // pour delete

	_errors[301] ="Moved permanently";  //redirection

	_errors[400] ="Bad Request";  
	_errors[401] ="Unauthorized";  //pour delete
	_errors[403] ="Forbidden";
	_errors[404] ="Not Found";
	_errors[405] ="Method Not Allowed";
	_errors[409] ="Conflict";
	_errors[410] ="Gone";
	_errors[413] ="Request Entity Too Large";

	_errors[500] ="Internal Server Error";
	_errors[503] ="Service Unavailable";
	_errors[505] ="HTTP Version non supported";  //a voir dans la requete
}

void	Response::initTypeMap()
{
	_types["php"] = "text/html";
	_types["aac"] = "audio/aac";
	_types["abw"] = "application/x-abiword";
	_types["arc"] = "application/octet-stream";
	_types["avi"] = "video/x-msvideo";
	_types["azw"] = "application/vnd.amazon.ebook";
	_types["bin"] = "application/octet-stream";
	_types["bmp"] = "image/bmp";
	_types["bz"] = "application/x-bzip";
	_types["bz2"] = "application/x-bzip2";
	_types["csh"] = "application/x-csh";
	_types["css"] = "text/css";
	_types["doc"] = "application/msword";
	_types["docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	_types["eot"] = "application/vnd.ms-fontobject";
	_types["epub"] = "application/epub+zip";
	_types["gif"] = "image/gif";
	_types["htm"] = "text/html";
	_types["html"] = "text/html";
	_types["ico"] = "image/x-icon";
	_types["ics"] = "text/calendar";
	_types["jar"] = "application/java-archive";
	_types["jpeg"] = "image/jpeg";
	_types["jpg"] = "image/jpeg";
	_types["js"] = "application/javascript";
	_types["json"] = "application/json";
	_types["mid"] = "audio/midi";
	_types["midi"] = "audio/midi";
	_types["mpeg"] = "video/mpeg";
	_types["mpkg"] = "application/vnd.apple.installer+xml";
	_types["odp"] = "application/vnd.oasis.opendocument.presentation";
	_types["ods"] = "application/vnd.oasis.opendocument.spreadsheet";
	_types["odt"] = "application/vnd.oasis.opendocument.text";
	_types["oga"] = "audio/ogg";
	_types["ogv"] = "video/ogg";
	_types["ogx"] = "application/ogg";
	_types["otf"] = "font/otf";
	_types["png"] = "image/png";
	_types["pdf"] = "application/pdf";
	_types["ppt"] = "application/vnd.ms-powerpoint";
	_types["pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	_types["rar"] = "application/x-rar-compressed";
	_types["rtf"] = "application/rtf";
	_types["sh"] = " application/x-sh";
	_types["svg"] = "image/svg+xml";
	_types["swf"] = "application/x-shockwave-flash";
	_types["tar"] = "application/x-tar";
	_types["tif"] = "image/tiff";
	_types["tiff"] = "image/tiff";
	_types["ts"] = "application/typescript";
	_types["ttf"] = "font/ttf";
	_types["vsd"] = "application/vnd.visio";
	_types["wav"] = "audio/x-wav";
	_types["weba"] = "audio/webm";
	_types["webm"] = "video/webm";
	_types["webp"] = "image/webp";
	_types["woff"] = "font/woff";
	_types["woff2"] = "font/woff2";
	_types["xhtml"] = "application/xhtml+xml";
	_types["xls"] = "application/vnd.ms-excel";
	_types["xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	_types["xml"] = "application/xml";
	_types["xul"] = "application/vnd.mozilla.xul+xml";
	_types["zip"] = "application/zip";
	_types["3gp"] = "video/3gpp";
	_types["3g2"] = "video/3gpp2";
	_types["7z"] = "application/x-7z-compressed";
}

void	Response::call(Request &request)
{
	_displayAutoindex = false;
	_request = request;
	_context = request.get_context(); 
	initErrorMap();
	initTypeMap();
	checkRequest();
	if (_context.get_httpRedirection().first == 301)
	{
		redirection();
	}
	else if (_statusCode == "200" && _request.get_methode() == "POST")
		postMethode();
	else if(_statusCode == "200" && _request.get_methode() == "DELETE")
		deleteMethode();
	else 
		getMethode();
}   

void 	Response::checkRequest()
{
	std::map<std::string, bool> tmp = _context.get_methodes();
	std::map<std::string, bool>::iterator it = tmp.find(_request.get_methode());
	if (it == tmp.end() || it->second == false)
		set_status(405);
	if (_request.get_version() != "HTTP/1.1")
		set_status(505);
	size_t maxSize = ft_atoi(_context.get_maxBodySize().c_str());
	if (maxSize && (_request.get_body().size() > maxSize))  //initialiser maxsize a combien?
		set_status(413);
	return ;
}

// -------------SETTER-------------------
void	Response::set_body(std::string path)
{
	if (path[0] == '/')
		path = path.erase(0, 1);
	if (_displayAutoindex == true)
	{
		_body = createAutoindex();
		return;
	}
	std::ofstream		file;
	std::stringstream	buffer;
	file.open(path.c_str(), std::ifstream::in);
	buffer << file.rdbuf();
	file.close();
	_body = buffer.str();
	
}

void		Response::set_cgibody(Request &request, std::string path)
{
	std::ofstream		file;
	std::stringstream	buffer;
	file.open(path.c_str(), std::ifstream::in);
	buffer << file.rdbuf();
	file.close();
	std::string tmp = buffer.str();
	request.set_body(tmp);
}

std::string	Response::set_contentType()
{
	if (_displayAutoindex == true)
		return ("text/html");
	int position = _path.find_last_of(".");
	std::string extension = _path.substr(position + 1);
	if (_types.find(extension) != _types.end())
		return (_types.find(extension)->second);
	return ("text/plain");
}

std::string	Response::set_contentLength()
{	
	return (std::string(ft_itoa(_body.size())));
}

std::string	Response::set_allow()
{
	std::map<std::string, bool>	methodes = _context.get_methodes();
	std::string ret;
	for(std::map<std::string, bool>::iterator it = methodes.begin(); it != methodes.end(); it++)
	{
		if (it->second == true)
			ret += it->first + " ";
	}
	return ret;
}

void	Response::set_headers()
{
	_headers["Content-Length:"] = set_contentLength();
	if (_statusCode == "405")
		_headers["Allow:"] = set_allow();
}

void	Response::set_response()   ///construire la reponse
{
	_strResponse = "HTTP/1.1 " + _statusCode + " " + _reasonPhrase + "\r\n"; ///first line
	for (std::map<std::string ,std::string>::iterator it =_headers.begin(); it!= _headers.end(); ++it) ///chaque header
		_strResponse += it->first + " " + it->second + "\r\n";
	_strResponse += "\r\n" + _body;
}

void	Response::set_status(int code) 
{ 
	_statusCode = std::string(ft_itoa(code));
	_reasonPhrase = _errors.find(code)->second;
}

/*************************************************************/
/*************        REDIRECTION     ************************/
/*************************************************************/

void	Response::redirection()
{
	set_status(301);
	_headers["Location:"] = _context.get_httpRedirection().second;
	_body = "";
	set_response();
}


/*************************************************************/
/*****************        POST     ***************************/
/*************************************************************/

void	Response::postMethode()
{
	_path = _request.get_context().get_root() + _request.get_path();
	size_t pos = _path.find(_context.get_locationPath());
	if(pos != std::string::npos)
		_path = _path.erase(pos, _context.get_locationPath().length());
	if (_path[0] == '/')
		_path = _path.erase(0, 1);
	_headers["Content-Type:"] = set_contentType();	
	_request.set_contentType(_headers["Content-Type:"]);
	if (_context.get_locationPath() == "/upload")
	{
		std::string name;
		size_t pos = _path.find_last_of("/");
		name = _context.get_upload_pass() + _path.substr(pos);
		std::ofstream output(name.c_str());
		output << _request.get_body();
		output.close();
		set_status(201);
		_body = "";
	}
	if (_context.get_cgi_pass() != "")
	{
		Cgi my_cgi(_request, _path);
		std::istringstream strResponse(my_cgi.execCgi(_context.get_cgi_pass(), _path));
		std::string str;
		while (getline(strResponse, str) && str != "\r")
		{
			std::stringstream ss(str);
			std::string key;
			std::string value;
			ss >> key;
			value.assign(str, key.size() + 1, str.size() - key.size() + 1);
			if (key == "Content-Type:")
				_headers[key] = value;
			if (key == "Status: ")
				set_status(ft_atoi(value.c_str()));
		}
		while (getline(strResponse, str))
			_body += str + "\n";
		if (_body == "Error")
			set_body(_context.get_error_page() + "/500.html");
	}
	else
	{
		set_status(204);
		_body = "";
	}
	set_headers();
	set_response();
}

/*************************************************************/
/****************        DELETE     **************************/
/*************************************************************/

void	Response::deleteMethode()
{
	_path = _request.get_context().get_root() + _request.get_path();
	size_t pos = _path.find(_context.get_locationPath());
	if(pos != std::string::npos)
		_path = _path.erase(pos, _context.get_locationPath().length());
	if (_path[0] == '/')
		_path = _path.erase(0, 1);
	std::cout << "PATH = " << _path << std::endl;
	struct stat mystat;
	errno = 0;
	stat(_path.c_str(), &mystat);
	if (errno)
	{
		set_status(404);
		perror("stat failed :");
		set_body(_context.get_error_page() + "/404.html");
	}
	else if (mystat.st_mode & S_IFDIR)
	{
		set_status(403);
		set_body(_context.get_error_page() + "/403.html");
	}
	else if (!(mystat.st_mode & S_IWUSR))
	{
		set_status(401);
		set_body(_context.get_error_page() + "/401.html");
	}
	else if ((remove(_path.c_str())) < 0)
	{
		set_status(401);
		perror("remove failed :");
		set_body(_context.get_error_page() + "/401.html");
	}
	else
	{
		set_status(204);
		_body = "";
	}	
	set_headers();
	set_response();
}

/*************************************************************/
/*****************         GET     ***************************/
/*************************************************************/
 
void	Response::getMethode()
{
	if (_statusCode != "200")
		_path = _context.get_error_page() + "/" + _statusCode + ".html";
	else
	{
		_path = _request.get_context().get_root() + _request.get_path();
		size_t pos = _path.find(_context.get_locationPath());
		if(pos != std::string::npos)
			_path = _path.erase(pos, _context.get_locationPath().length());
		set_path();
	}
	_headers["Content-Type:"] = set_contentType();
	_request.set_contentType(_headers["Content-Type:"]);
	if (_context.get_cgi_pass() != "" && _statusCode == "200")
	{
		set_cgibody(_request, _path);
		Cgi my_cgi(_request, _path);
		std::istringstream strResponse(my_cgi.execCgi(_context.get_cgi_pass(), _path));
		std::string str;
		while (getline(strResponse, str) && str != "\r")
		{
			std::stringstream ss(str);
			std::string key;
			std::string value;
			ss >> key;
			value.assign(str, key.size(), str.size() - key.size());
			if (key == "Content-Type: ")
				_headers[key] = value;
			if (key == "Status: ")
				set_status(ft_atoi(value.c_str()));
		}
		while (getline(strResponse, str))
			_body += str + "\n";
		if (_body == "Error")
			set_body(_context.get_error_page() + "/500.html");
	}
	else
		set_body(_path);
	set_headers();
	set_response();
}

void	Response::set_path()
{
	if (_path[0] == '/')
		_path = _path.erase(0, 1);
	struct stat mystat;
	errno = 0;
	stat(_path.c_str(), &mystat);
	if (errno)
	{
		set_status(404);
		_path = _context.get_error_page() + "/404.html";
		return;
	}
	if (mystat.st_mode & S_IFDIR)
	{
		if (_path[_path.size() - 1] != '/')
		{
			set_status(404);
			_path = _context.get_error_page() + "/404.html";
			return;
		}
		if (_context.get_autoindex() == false)
		{
			if (_context.get_index() == "")
			{
				set_status(404);
				_path = _context.get_error_page() + "/404.html";
				return;
			}
			else if (checkFile(_path + _context.get_index()) == 404)
			{
				set_status(404);
				_path = _context.get_error_page() + "/404.html";
				return ;
			}
			_path += _context.get_index();
		}
		if (_context.get_autoindex() == true)
		{
			if (_context.get_index() == "")
			{
				_displayAutoindex = true;
				return;
			}
			else if (checkFile(_path + _context.get_index()) == 404)
			{
				_displayAutoindex = true;
				return ;
			}
			_path += _context.get_index();
		}
	}

}

int Response::checkFile(std::string path)
{
	struct stat		sb;
	if (stat(path.c_str(), &sb) < 0)
	{
		set_status(404);
		return 404;
	}
	return 0;
}

std::string	Response::createAutoindex()
{
    DIR *diropen = opendir(_path.c_str());
    if (diropen == NULL) 
	{
        std::cerr << "opendir failed" << std::endl;
        return NULL;
	}
    std::string str =\
	"<!DOCTYPE html>\n\
    <html>\n\
    <head>\n\
    <title> Index of " + _request.get_path() + "</title>\n\
    </head>\n\
    <body>\n\
    <h1>Index of " + _request.get_path() + " </h1>\n\
    <p>\n\
	<a href=\"../\">../</a><br>\n";
	for (struct dirent *dentry = readdir(diropen); dentry; dentry = readdir(diropen)) 
	{
		std::stringstream tmp;
		tmp << dentry->d_name;
		if (tmp.str() != "." && tmp.str() != "..")
		{
			std::stringstream ss;
			if (dentry->d_type == DT_DIR)
			{
				ss << "<a href=\"http://localhost:" << _request.get_context().get_port() << _request.get_path()\
				<< dentry->d_name << "/\">" << dentry->d_name << "</a><br>\n";
				str += ss.str();
			}
			else
			{
				ss << "<a href=\"http://localhost:" << _request.get_context().get_port() << _request.get_path()\
				<< dentry->d_name << "\">" << dentry->d_name << "</a><br>\n";
				str += ss.str();
			}
		}
	}
    str +="\
    </p>\n\
    </body>\n\
    </html>\n";
    closedir(diropen);
	return str;
}

// -------------GETTER-------------------
std::string	Response::get_response()const{ return _strResponse;}
Request		Response::get_request()const{ return _request;}
std::string Response::get_path()const{ return _path;}
std::string Response::get_httpVersion()const{ return _httpVersion;}
std::string Response::get_statusCode()const{ return _statusCode;}
std::string Response::get_reasonPhrase()const{ return _reasonPhrase;}
std::string Response::get_contentType()const{ return _contentType;}
std::string Response::get_contentLength()const{ return _contentLength;}
std::string Response::get_body()const{ return _body;}
std::map<int, std::string> Response::get_errors()const{ return _errors;}
std::map<std::string, std::string> Response::get_types()const{ return _types;}
std::map<std::string, std::string> Response::get_headers()const{ return _headers;}
bool Response::get_displayAutoindex()const{ return _displayAutoindex;}
Context		Response::get_context()const{ return _context;}