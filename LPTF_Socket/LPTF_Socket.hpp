#ifndef LPTF_SOCKET_HPP
#define LPTF_SOCKET_HPP

#include <string>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <vector>
#include <algorithm>

class LPTF_Socket {
private:
    int socket_fd;
    struct sockaddr_in address;
    std::vector<int> clientSockets;
    int port;
    in_addr_t ipAddr;


public:
    LPTF_Socket(int port) : socket_fd(-1), port(port) {};
    bool initClient(const char* server_ip);
    bool initServer();
    int acceptConnection();
    bool send(int clientSocket_fd, const char* data);
    void closeSocket();
    void displayServerAddress();
    void removeClient(int clientSocket);
    void launchServer();
    bool launchClient();
    void handleNewConnection(fd_set& readfds);
    void handleClientCommunication(int clientSocket);
    std::string receiveClient();
};

#endif // LPTF_SOCKET_HPP