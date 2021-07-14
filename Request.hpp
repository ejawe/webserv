#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "include.hpp"
#include "Context.hpp"
#include <map>

class Request
{

public:

    Request();
    Request(Request const &cp);
    Request &operator=( Request const & rhs );
    ~Request();

    void    call(std::string str, Context &context);
    void    pars();
    void    pars_url(std::string path);
    void    clear();
    
    // -------------SETTER-------------------
    void    set_context(Context &context, std::string path);
    void    set_contentType(std::string type);
    void    set_body(std::string &body);

    // -------------GETTER-------------------
    std::string get_methode();
    std::string get_path();
    std::string get_version();
    std::map<std::string, std::string> get_headers();
    std::string get_body();
    int         get_statusCode();
    Context     get_context();
    std::string get_uri();
    std::string get_query();
    std::string get_contentType();

    void	    printRequest();
private:

    std::string                         _strRequest;
    std::string                         _methode;
    std::string                         _path;
    std::string                         _version;
    std::map<std::string, std::string>  _headers;
    std::string                         _body;
    int                                 _statusCode;
    Context                             _context;
    std::string                         _query;
    std::string                         _contentType;
    std::string                         _uri;
};

#endif