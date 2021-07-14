#include "include.hpp"
#include "Cluster.hpp"

int		main(int ac,char **av)
{
	Cluster cluster;
	if (ac != 2)
	{
		std::cout << "Wrong number of argument\n";
		return 0;
	}
	std::string path = std::string(av[1]);
	if ((cluster.clusterConfig(path)) < 0)
	{
		std::cout << "Error: config file\n";
		return (0);
	}
	try
	{
		cluster.clusterSetup();
		cluster.clusterRun();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	return 0;
}



















// #include "response.hpp"
// #include "server.hpp"
// #include "include.hpp"

// int		g_socket;
// int		g_size_add;
// void	*g_add;

// void	closeserver(int nb)
// {
// 	(void)nb;
// 	ft_memset(g_add, 0, g_size_add);
//     close(g_socket);
// 	exit(0);
// }

// int		main()
// {
// 	// Server 			server;

// 	// int				new_socket = -1;
// 	// int				timeout = (3 * 60 * 1000); // 1 min
// 	// struct pollfd 	fds[POLL_SIZE];
// 	// int    			nfds = 1;
// 	// int				current_size = 0;
// 	// int				end_server = FALSE;
// 	// int				compress_array = FALSE;
// 	// int				close_conn;
// 	// int 			ret = 0;

// 	// signal(SIGINT, closeserver);
// 	try
// 	{
// 		// server.setup();

// 		// g_socket =  server.get_sfd();
// 		// g_add = server.get_address();
// 		// g_size_add = sizeof(*server.get_address());

// 		// server.listenConnexion();

	
// 	}
// 	catch(const std::exception& e)
// 	{
// 		std::cerr << e.what() << '\n';
// 	}
// 	return 0;
// }