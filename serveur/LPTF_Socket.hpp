#ifndef LPTF_SOCKET_HPP
#define LPTF_SOCKET_HPP

#include <string>

class LPTF_Socket {
private:
    int sockfd;

public:
    LPTF_Socket();
    ~LPTF_Socket();
    bool connectTo(const char* address, int port);
    bool bindAndListen(int port);
    int acceptConnection();
    bool sendMessage(const char* message);
    std::string receiveMessage();
    void displayServerAddress();
    // Ajoutez d'autres méthodes nécessaires ici
};

#endif // LPTF_SOCKET_HPP
