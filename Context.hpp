#ifndef CONTEXT_HPP
# define CONTEXT_HPP

#include "include.hpp"

class Context
{  
    public:
    Context();
	Context(Context const &cp);
	Context &operator=(Context const &cp);
	~Context();

	int								contextConfig(std::string scope);
	bool							checkMethode(std::string methode);
	void							addLocation(std::string path, Context *location);
	void                        	copyAttribut(Context &cp);
	void							contextClear();
	void							copyslash(Context const &cp);
	int								check_context();

    // -------------GETTER-------------------
	// std::string						get_host() const;
	int								get_port() const;
	std::string						get_server_name() const;
	std::string						get_maxBodySize() const;
	std::string						get_error_page() const;
	std::map<std::string, bool>		get_methodes() const;
	std::string						get_root() const;
	bool							get_autoindex() const;
	std::string						get_index() const;
    std::string						get_cgi() const;
    std::string						get_cgi_pass() const;
    std::string						get_upload_pass() const;
	std::pair<int, std::string>		get_httpRedirection() const;
    std::map<std::string, Context *>get_locations() const;
	std::string						get_locationPath() const;

    // -------------SETTER-------------------
	void	set_contextAttribut(std::string &str);
	void	set_listen(std::string &str); ////set le port et l'host
	void	set_server_name(std::string &str);
	void	set_maxBodySize(std::string &str);
	void	set_erro_page(std::string &str);
	void	set_methode(std::string &str);
	void	set_root(std::string &str);
	void	set_autoindex(std::string &str);
	void	set_index(std::string &str);
    void	set_cgi(std::string &str);
    void	set_cgi_pass(std::string &str);
    void	set_upload_pass(std::string &str);
	void	set_httpRedirection(std::string &str);
	void	set_locationPath(std::string &str);  ///valable que pour les context de location

	void	printContext();

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
	// std::string						_host;
	int								_port;
	std::string						_server_name;
	std::string						_maxBodySize;
	std::string						_error_page;
	std::map<std::string, bool>		_methodes;
	std::string						_root;
	bool							_autoindex;
	std::string						_index;
    std::string						_cgi;
    std::string						_cgi_pass;
    std::string						_upload_pass;
	std::string						_locationPath;
	std::pair<int, std::string>		_httpRedirection;
    std::map<std::string, Context*>	_locations;

};

#endif