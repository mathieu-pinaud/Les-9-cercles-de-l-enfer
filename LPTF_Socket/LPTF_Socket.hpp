#ifndef LPTF_SOCKET_HPP
#define LPTF_SOCKET_HPP



#include "../LPTF_Packet/LPTF_Packet.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <ifaddrs.h>
#include <algorithm>

class LPTF_Socket {
private:
    int socket_fd;
    struct sockaddr_in address;
    std::vector<int> clientSockets;
    int port;

public:
    LPTF_Socket(int port) : socket_fd(-1), port(port) {};
    

    void addClientSocket(int clientSocket) { clientSockets.push_back(clientSocket); }
    std::vector<int> getClientSockets() { return clientSockets; }

    bool send(int clientSocket_fd, const LPTF_Packet packet);
    void closeSocket();
    LPTF_Packet receivePacket();

    int getSockFd() const { return socket_fd; }
    void setSockFd(const int sock_fd) { socket_fd = sock_fd; }
    struct sockaddr_in& getAddress() { return address; }
    struct sockaddr_in* testGetAddress() { return &address; }
    void setAddress(struct sockaddr_in adr) { address = adr; }
    int getPort() const { return port; }
    void setPort(const int portIn) { port = portIn; }
    void setAddressFamily(int family) { address.sin_family = family; }
    void setAddressPort(int port) { address.sin_port = port; }
    void setAddressAdr(in_addr_t adress) { address.sin_addr.s_addr = adress; }
    bool setAddressIp(const char* ip) { return inet_pton(AF_INET, ip, &address.sin_addr) > 0; }
    const char* getAddressIp() {static char ip[INET_ADDRSTRLEN];return inet_ntop(AF_INET, &(address.sin_addr), ip, INET_ADDRSTRLEN);}
    int getAddressPort() { return ntohs(address.sin_port); }
   
};

#endif // LPTF_SOCKET_HPP