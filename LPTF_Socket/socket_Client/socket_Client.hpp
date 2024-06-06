#ifndef SOCKET_CLIENT_HPP
#define SOCKET_CLIENT_HPP

#include "../LPTF_Socket.hpp"

class socket_Client : public LPTF_Socket {

    public:
        socket_Client(int port) : LPTF_Socket(port) {};
        bool initClient(const char* server_ip);
        bool launchClient();
};

#endif