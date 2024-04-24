#include "../LPTF_Socket/LPTF_Socket.hpp"
#include <iostream>
#include <cstring> // Pour std::strlen

int checkArgs(int ac, char* av[]) {
    if (ac != 3) {
        std::cerr << "Usage: " << av[0] << " <ip> <port>" << std::endl;
        return false;
    }
    int port = -1;
    try {
        port = std::stoi(av[2]);
    } catch (const std::exception& e) {
        std::cerr << "Le port doit être un entier." << std::endl;
        return false;
    }
    if (port < 1024 || port > 49151) {
        std::cerr << "Le port doit être compris entre 1024 et 49151." << std::endl;
        return false;
    }
    return port;
}


int main(int argc, char* av[]){
    int port = checkArgs(argc, av);
    if (port == -1) {
        return 1;
    }
    LPTF_Socket clientSocket(port);
    if (clientSocket.initClient(av[1])) {
        std::cout << "Connecté au serveur." << std::endl;

        while (true) {
            std::string message;
            std::getline(std::cin, message);
            if (message == "stop") {
                std::cout << "Fermeture de la connexion..." << std::endl;
                break;
            }
            if (!clientSocket.send(clientSocket.getSocket(), message.c_str())) {
                std::cerr << "Erreur lors de l'envoi du message au serveur." << std::endl;
                break;
            }
        }
    } else {
        std::cerr << "Échec de la connexion au serveur." << std::endl;
        return 1;
    }
    clientSocket.closeSocket();
    return 0;
}