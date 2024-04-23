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
    // Ajoutez d'autres méthodes nécessaires ici
    bool sendMessage(const char* message);
    std::string receiveMessage();
};

#endif // LPTF_SOCKET_HPP