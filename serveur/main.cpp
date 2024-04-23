#include "LPTF_Socket.hpp"
#include <iostream>
#include <cstring> // Pour std::strlen
#include <vector>

// Fonction pour gérer la communication avec un client
void handleClientCommunication(LPTF_Socket& client, std::vector<LPTF_Socket>& clients) {
    while (true) {
        std::string message = client.receiveMessage();
        if (message.empty()) {
            std::cerr << "Erreur lors de la réception du message." << std::endl;
            break;
        }
        std::cout << "Message reçu du client : " << message << std::endl;
        for (auto& otherClient : clients) {
            if (&otherClient != &client) {
                if (!otherClient.sendMessage(message.c_str())) {
                    std::cerr << "Erreur lors de l'envoi du message à un client." << std::endl;
                    break;
                }
            }
        }
    }
}

// Fonction pour gérer l'acceptation des connexions clientes
void handleClientConnections(LPTF_Socket& serverSocket) {
    std::vector<LPTF_Socket> clients;

    while (true) {
        int clientSocket = serverSocket.acceptConnection();
        if (clientSocket >= 0) {
            std::cout << "Connexion acceptée." << std::endl;
            clients.emplace_back();
            LPTF_Socket& client = clients.back();
            if (!client.connectTo("127.0.0.1", 12345)) { // Remplacer les paramètres d'adresse et de port par les valeurs appropriées
                std::cerr << "Erreur lors de la connexion du client." << std::endl;
                continue;
            }
            for (auto& otherClient : clients) {
                if (&otherClient != &client) {
                    if (!otherClient.sendMessage("Un nouveau client s'est connecté.")) {
                        std::cerr << "Erreur lors de l'envoi du message à un client." << std::endl;
                        break;
                    }
                }
            }
            handleClientCommunication(client, clients);
        } else {
            std::cerr << "Erreur lors de l'acceptation de la connexion." << std::endl;
        }
    }
}

int main() {
    LPTF_Socket serverSocket;
    if (serverSocket.bindAndListen(12345)) {
        std::cout << "Serveur en attente de connexions entrantes." << std::endl;

        handleClientConnections(serverSocket);
    } else {
        std::cerr << "Erreur lors de la liaison et de l'écoute." << std::endl;
        return 1;
    }

    return 0;
}
