#include "LPTF_Socket.hpp"
#include <iostream>
#include <cstring> // Pour std::strlen

int main() {
    LPTF_Socket clientSocket;
    if (clientSocket.connectTo("10.10.3.4", 12345)) { // Remplacer les paramètres d'adresse et de port par les valeurs appropriées
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
