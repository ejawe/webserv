#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "include.hpp"
#include "CGI.hpp"
#include "Request.hpp"
#include "Context.hpp"



class Response
{

public:
	Response();
	Response(Response const &cp);
	Response &operator=(Response const &cp);
	~Response();

	int			checkFile(std::string path);
	void		call(Request &src);
	void		initErrorMap();
	void		initTypeMap();
	void		checkRequest();
	void		getMethode();
	void		deleteMethode();
	void		postMethode();
	std::string	createAutoindex();
	void    	redirection();


	// -------------SETTER-------------------
	void	set_version();
	void	set_status(int code);
	void	set_headers();
	void	set_response();
	void	set_body(std::string path);
	void	set_path();
	std::string	set_allow();
	std::string	set_contentType();
	std::string	set_contentLength();
	void		set_cgibody(Request &request, std::string path);

	// -------------GETTER-------------------
	std::string get_response()const;
	Request		get_request()const;
	std::string get_path()const;
	std::string get_httpVersion()const;
	std::string get_statusCode()const;
	std::string get_reasonPhrase()const;
	std::string get_contentType()const;
	std::string get_contentLength()const;
	std::string get_body()const;
	std::map<int, std::string> get_errors()const;
	std::map<std::string, std::string> get_types()const;
	std::map<std::string, std::string> get_headers()const;
	bool get_displayAutoindex()const;
	Context		get_context()const;

	//EXCEPTION
	class ErrorException: public std::exception
    {
    public:
        ErrorException(std::string const& phrase="") throw() : m_phrase(phrase)
        {}
        virtual const char* what() const throw()
        {return m_phrase.c_str();}  
        virtual ~ErrorException() throw() {}

    private:
        std::string m_phrase;
    };

private:
	Request		_request;
	std::string _path;
	std::string _httpVersion;
	std::string _statusCode;
	std::string _reasonPhrase;
	std::string _contentType;
	std::string _contentLength;
	std::string _body;
	std::string	_strResponse;
	std::map<int, std::string> _errors;
	std::map<std::string, std::string> _types;
	std::map<std::string, std::string> _headers;
	bool _displayAutoindex;
	Context		_context;
};

#endif