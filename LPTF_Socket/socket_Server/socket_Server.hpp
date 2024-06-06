#ifndef SOCKET_SERVER_HPP
#define SOCKET_SERVER_HPP

#include "../LPTF_Socket.hpp"

class socket_Server : public LPTF_Socket {

    public:
        socket_Server(int port) : LPTF_Socket(port) {};
        bool initServer();
        int acceptConnection();
        void displayServerAddress();
        void removeClient(int clientSocket);
        void launchServer();
        void handleNewConnection(fd_set& readfds);
        void handleClientCommunication(int clientSocket);
};

#endif