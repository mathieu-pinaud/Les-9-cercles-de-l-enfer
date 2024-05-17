#include "../LPTF_Socket/LPTF_Socket.hpp"
#include <vector>


int checkArgs(int ac, char* av[]) {
    if (ac != 2) {
        std::cerr << "Usage: " << av[0] << " <port>" << std::endl;
        return -1;
    }
    int port = -1;
    try {
        port = std::stoi(av[1]);
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

int main(int ac, char* av[]) {
    int port = checkArgs(ac, av);
    if (port == -1)
        return 1;
    LPTF_Socket serverSocket(port);
    if (!serverSocket.initServer()) {
        std::cerr << "Failed to initialize server." << std::endl;
        return 1;
    }
    serverSocket.launchServer();
}