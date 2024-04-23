#ifndef LPTF_SOCKET_HPP
#define LPTF_SOCKET_HPP

#include <string>

class LPTF_Socket {
private:
    int sockfd;
    const std::string ipAddr;

public:
    LPTF_Socket();
    ~LPTF_Socket();
    bool connectTo(const char* address, int port);
    bool bindAndListen(int port);
    int acceptConnection();
    bool sendMessage(const char* message);
    std::string receiveMessage();
    void displayServerAddress();
    std::string getIpAddr() const { return ipAddr; };
};

#endif // LPTF_SOCKET_HPP
