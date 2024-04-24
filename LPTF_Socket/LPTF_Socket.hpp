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

class LPTF_Socket {
private:
    int socket_fd;
    std::string ipAddr;
    struct sockaddr_in address;
    std::vector<int> clientSockets;
    int port;


public:
    LPTF_Socket(int port) : socket_fd(-1) {}
    bool initClient(const char* server_ip);
    bool initServer();
    int acceptConnection();
    bool send(int clientSocket_fd, const char* data);
    ssize_t receive(char* buffer, int buffer_size);
    void closeSocket();
    void displayServerAddress();
    std::string getIpAddr() const { return ipAddr; }
    std::vector<int> getClientSockets() const { return clientSockets; }
    void addClient(int clientSocket) { clientSockets.push_back(clientSocket); }
    void removeClient(int clientSocket);
    void clearClientSockets() { clientSockets.clear(); }
    int getSocket() const { return socket_fd; }
    int getPort() const { return port; }
    void launchServer();
    void launchClient(const char* server_ip);
    void handleNewConnection(fd_set& readfds);
    void handleClientCommunication(int clientSocket);
};

#endif // LPTF_SOCKET_HPP