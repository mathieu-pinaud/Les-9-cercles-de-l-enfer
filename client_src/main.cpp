#include "../LPTF_Socket/socket_Client/socket_Client.hpp"
#include "../LPTF_Socket/LPTF_Socket.hpp"
#include <iostream>
#include <cstring> // Pour std::strlen

int checkArgs(int ac, char* av[]) {
    if (ac != 3) {
        std::cerr << "Usage: " << av[0] << " <ip> <port>" << std::endl;
        return -1;
    }
    int port = -1;
    try {
        port = std::stoi(av[2]);
    } catch (const std::exception& e) {
        std::cerr << "Le port doit être un entier." << std::endl;
        return -1;
    }
    if (port < 1024 || port > 49151) {
        std::cerr << "Le port doit être compris entre 1024 et 49151." << std::endl;
        return -1;
    }
    return port;
}


int main(int argc, char* av[]){
    int port = checkArgs(argc, av);
    if (port == -1) {
        return 1;
    }
    socket_Client clientSocket(port);
    if (clientSocket.initClient(av[1])) {
        clientSocket.closeSocket();
    } else {
        std::cerr << "Échec de la connexion au serveur." << std::endl;
        return 1;
    }
    clientSocket.closeSocket();
    return 0;
}