
#ifndef CLUSTER_HPP
# define CLUSTER_HPP

#include "include.hpp"
#include "Server.hpp"

class Cluster
{

public:
    Cluster();
    Cluster(Cluster const &cp);
    Cluster &operator=(Cluster const &cp);
    ~Cluster();

    void    clusterSetup();
    void    clusterRun();
    // void    clusterClean();

    // -------------GETTER-------------------
    std::vector<Server *>     get_servers() const;
    // -------------SETTER-------------------
    // -------------EXECPTION----------------

    // -------------CONFIGURATION-----------------
    int     clusterConfig(std::string &path);
    int     checkFile(std::string path);
    void    serversClear();

    void    printClusterConfig();

private:
    std::vector<Server *>  _servers;
    struct pollfd 	_fds[200]; ///a voir
    int             _nfds;
};



#endif