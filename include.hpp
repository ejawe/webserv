#ifndef UTILS_HPP
# define UTILS_HPP

#include <string.h>
#include <iostream>
#include <exception>
#include <algorithm>
#include <errno.h>
//Socket . Bind . Listen . Accept
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
//sockaddr_in
#include <netinet/in.h>
//memset
#include <string.h>
//read . write
#include <unistd.h>

#include <signal.h>

#include <sstream>

#include <fstream>
#include <sys/stat.h>

//select
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

//stl

#include <map>
#include <vector>
#include <utility>
#include <algorithm>

#include <fcntl.h>

// poll
#include <poll.h>

//opendir
#include <dirent.h>

#include <sys/wait.h>

// Colors
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define ENDCOLOR "\033[0m"

#define RECV_SIZE	65536
#define TRUE		1
#define FALSE		0
#define POLL_SIZE	200

void	*ft_memset(void *s, int c, size_t n);

int	ft_atoi(const char *str);
std::string		ft_getNextDir(std::string *path);
std::string ft_itoa(int n);

#endif