#ifndef LPTF_SOCKET_HPP
#define LPTF_SOCKET_HPP


#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#elif __linux__
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
#else
    #error "OS not supported"
#endif

#include <string>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <algorithm>

class LPTF_Socket {
private:
    int socket_fd;
    struct sockaddr_in address;
    std::vector<int> clientSockets;
    int port;
    #ifdef _WIN32
        WSADATA ipAddr;
    #elif __linux__
        in_addr_t ipAddr;
    #endif

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