#ifndef CGI_HPP
# define CGI_HPP

#include "include.hpp"
#include "Request.hpp"
#include "Context.hpp"

class Cgi 
{
	public:
		Cgi(Request &request, std::string path);
		Cgi(Cgi const &cp);
		Cgi &operator=(Cgi const &cp);
		~Cgi();


		std::string		execCgi(const std::string &scriptName, std::string &path);
		void			initEnv(Request &request, std::string path);
		//////getter////
		char**				get_env()const;
		std::string			get_body()const;
        Context             get_context()const;
		Request				get_request()const;
	private:
	    Cgi(void);
		char**				_env;
		std::string			_body;
        Context             _context;
		Request				_request;
};

#endif