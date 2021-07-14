#ifndef SERVER_HPP
# define SERVER_HPP

#include "include.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include "Context.hpp"

class Server
{

public:

    Server();
    ~Server();

    void            setup();
    void            listenConnexion();
    int             acceptConnexion(int new_socket);
    int             receive(int new_socket);
    int             respond(int new_socket);
    int		        check_contex();

    // -------------GETTER-------------------
    int              get_sfd() const;
    sockaddr_in      *get_address();
    Context          *get_context();
    int              get_port() const;
    std::vector<int> get_sockets() const;
    // -------------SETTER-------------------
    void			set_port();
    // -------------EXECPTION-----------------
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
    // -------------CONFIGURATION-----------------
    int		serverConfig(std::string &scope);
    void    printConfig(){_context.printContext();}

private:

    struct sockaddr_in  _address;
	int                 _PORT;
    int                 _sfd;
    // Response            _response;
    Request             _request;
    std::vector<int>    _sockets;
    Context             _context;   //context general du server. contien les context pour chaque location  

    Server(Server const &cp);
    Server &operator=(Server const &cp);
};

#endif