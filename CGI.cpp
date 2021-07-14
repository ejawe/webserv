
#include "CGI.hpp"

Cgi::Cgi(){}

Cgi::Cgi(Request &request, std::string path)
{
	_request = request;
    _context = request.get_context();
    _body = request.get_body();
    initEnv(request, path);
}

Cgi::Cgi(Cgi const &cp){*this = cp;}
Cgi &Cgi::operator=(Cgi const &cp)
{
	if ( this != &cp )
	{
		this->_env = cp.get_env();
		this->_body = cp.get_body();
        this->_context = cp.get_context();
		this->_request = cp.get_request();
	}
	return *this;
}

Cgi::~Cgi()
{
}

void			Cgi::initEnv(Request &request, std::string path)
{
    std::map<std::string, std::string> 				env;
    std::map<std::string, std::string>	            headers = request.get_headers();
	int i = 0;

	env["SERVER_PROTOCOL"] = "HTTP/1.1";
	env["REDIRECT_STATUS"] = "200";
	if(_context.get_cgi() != ".php")
		env["REQUEST_METHOD"] = request.get_methode(); 
	// env["REMOT_ADDR"] = _context.get_host();
	// env["REMOT_ADDR"] = "_context.get_host()";
	char* pwd = NULL;
	if (!(pwd = getcwd(NULL, 0)))
	{
		perror("pwd() ");
		return ;
	}
	env["PATH_INFO"] = pwd + ("/" + path);
	env["PATH_TRANSLATED"] = request.get_path();
	env["CONTENT_LENGTH"] = ft_itoa(_body.size());
	env["QUERY_STRING"] = request.get_query();
	env["CONTENT_TYPE"] = request.get_contentType();
	env["SERVER_PORT"] = ft_itoa(_context.get_port());
    if (headers.find("Auth-Scheme") != headers.end() && headers["Auth-Scheme"] != "")
		env["AUTH_TYPE"] = headers["Authorization"];
	env["REMOTE_IDENT"] = headers["Authorization"];
	env["REMOTE_USER"] = headers["Authorization"];
	_env = (char **)malloc(sizeof(char *) * (env.size() + 1));
	for(std::map<std::string, std::string>::iterator it = env.begin(); it != env.end(); it++)
	{
		_env[i] = strdup((it->first + "=" + it->second).c_str());
		i++;
	}
	_env[i] = NULL;
	free(pwd);
}

std::string		Cgi::execCgi(const std::string &script, std::string &path)
{
	pid_t		pid;
	int			tmpin = dup(STDIN_FILENO);
	int			tmpout = dup(STDOUT_FILENO);
	std::string	new_body;
	char	buffer[RECV_SIZE] = {0};

	FILE	*fileIn = tmpfile(); 
	FILE	*fileOut = tmpfile();
	long	fdIn = fileno(fileIn);
	long	fdOut = fileno(fileOut);
	char	**args = NULL;

	char* pwd = NULL;
	if (!(pwd = getcwd(NULL, 0)))
	{
		perror("pwd() ");
		return ("ERROR");
	}
	std::string chemin = pwd + ("/" + script);
	args = (char **)(malloc(sizeof(char *) * 3));
	args[0] = strdup(chemin.c_str());
	std::string  name = pwd + ("/" + path);
	args[1] = strdup(name.c_str());
    args[2] = NULL;

	write(fdIn, _body.c_str(), _body.size());
	lseek(fdIn, 0, SEEK_SET); //aller au debut du fichier

	pid = fork();
	if (pid == -1)
	{
		perror("fork() ");
		return ("ERROR");
	}
	else if (pid == 0)
	{
		dup2(fdIn, STDIN_FILENO);
		dup2(fdOut, STDOUT_FILENO);
		if ((execve(args[0], args, _env)) < 0)
		{
			perror(" execv() ");
			write(STDOUT_FILENO, "ERROR", 5);
		}
		exit (0);
	}
	else
	{
		waitpid(-1, NULL, 0);
		lseek(fdOut, 0, SEEK_SET);
		while ((read(fdOut, buffer, RECV_SIZE - 1)))
		{
			new_body += buffer;
			memset(buffer, 0, RECV_SIZE);
		}
		dup2(tmpin, STDIN_FILENO); //recuper sorti/entree standard
		dup2(tmpout, STDOUT_FILENO);
	}
	///fermer et free
	close(tmpin);
	close(tmpout);
	fclose(fileIn);
	fclose(fileOut);
	close(fdIn);
	close(fdOut);
	for (size_t i = 0; _env[i]; i++)
		free(_env[i]);
	free(_env);
	for (size_t i = 0; args[i]; i++)
		free(args[i]);
	free(args);
	free(pwd);
	return (new_body);
}

char**				Cgi::get_env()const{return _env;}
std::string			Cgi::get_body()const{return _body;}
Context             Cgi::get_context()const{return _context;}
Request				Cgi::get_request()const{return _request;}