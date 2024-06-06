CC = g++
CFLAGS = -g3 -Wall -Wextra -Werror
NAME_SERVER = server
NAME_CLIENT = client
SRC_CLIENT = client_src/main.cpp LPTF_Socket/LPTF_Socket.cpp LPTF_Socket/socket_Client/socket_Client.cpp 
SRC_SERVER = server_src/main.cpp LPTF_Socket/LPTF_Socket.cpp LPTF_Socket/socket_Server/socket_Server.cpp
OBJ_SERVER = $(SRC_SERVER:.cpp=.o)
OBJ_CLIENT = $(SRC_CLIENT:.cpp=.o)

all: $(NAME_SERVER) $(NAME_CLIENT)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME_SERVER): $(OBJ_SERVER)
	$(CC) $(CFLAGS) -o ./$(NAME_SERVER) $(OBJ_SERVER)

$(NAME_CLIENT): $(OBJ_CLIENT)
	$(CC) $(CFLAGS) -o ./$(NAME_CLIENT) $(OBJ_CLIENT)

clean:
	rm -f $(OBJ_SERVER) $(OBJ_CLIENT)

fclean: clean
	rm -f $(NAME_SERVER) $(NAME_CLIENT)

re: fclean all

.PHONY: all clean fclean re