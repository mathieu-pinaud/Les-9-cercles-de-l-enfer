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
    LPTF_Socket(int port) : socket_fd(-1) {};
    bool initClient(const char* server_ip);
    bool initServer();
    int acceptConnection();
    bool send(int clientSocket_fd, const char* data);
    ssize_t receive(char* buffer, int buffer_size);
    void closeSocket();
    void displayServerAddress();
    #ifdef _WIN32
        WSADATA getIpAddr() const { return ipAddr; };
    #elif __linux__
        in_addr_t getIpAddr() const { return ipAddr; };
    #endif
    std::vector<int> getClientSockets() const { return clientSockets; };
    void addClient(int clientSocket) { clientSockets.push_back(clientSocket); };
    void removeClient(int clientSocket);
    void clearClientSockets() { clientSockets.clear(); };
    int getSocket() const { return socket_fd; };
    int getPort() const { return port; };
    void launchServer();
    void launchClient(const char* server_ip);
    void handleNewConnection(fd_set& readfds);
    void handleClientCommunication(int clientSocket);
};

#endif // LPTF_SOCKET_HPP