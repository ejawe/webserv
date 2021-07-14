NAME		=	webserv

SRCS		=	main.cpp \
				Cluster.cpp \
				Context.cpp \
				Request.cpp \
				Response.cpp \
				Server.cpp \
				utils.cpp \
				CGI.cpp

CC			=	clang++

OBJS		=	$(SRCS:.cpp=.o)

CPPFLAGS	=	-Wall -Wextra -Werror -std=c++98

all			:	$(NAME)

$(NAME)		:	$(OBJS)
				$(CC) -o $(NAME) $(CPPFLAGS) $(OBJS)

clean		:
				rm -rf $(OBJS)

fclean		:	clean
				rm -f $(NAME)

re			:	fclean all