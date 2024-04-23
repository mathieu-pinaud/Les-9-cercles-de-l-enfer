#include "LPTF_Socket.hpp"
#include <iostream>
#include <cstring> // Pour std::strlen

int main(int argc, char** __argv){
    if(argc != 3){
        std::cerr << "Usage: " << __argv[0] << " <ip> <port>" << std::endl;
        return 1;
    }
    char* ip = __argv[1];
    int port = atoi(__argv[2]);
    LPTF_Socket clientSocket;
    if (clientSocket.connectTo(ip, port)) { // Remplacer les paramètres d'adresse et de port par les valeurs appropriées
        std::cout << "Connecté au serveur." << std::endl;

        while (true) {
            std::string message;
            std::getline(std::cin, message);
            if (message == "stop") {
                std::cout << "Fermeture de la connexion..." << std::endl;
                break;
            }
            if (!clientSocket.sendMessage(message.c_str())) {
                std::cerr << "Erreur lors de l'envoi du message au serveur." << std::endl;
                break;
            }
        }
    } else {
        std::cerr << "Échec de la connexion au serveur." << std::endl;
        return 1;
    }

    return 0;
}
