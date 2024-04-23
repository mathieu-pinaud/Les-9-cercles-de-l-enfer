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
void handleClientConnections(LPTF_Socket& serverSocket, int port) {
    std::vector<LPTF_Socket> clients;

    while (true) {
        int clientSocket = serverSocket.acceptConnection();
        if (clientSocket >= 0) {
            std::cout << "Connexion acceptée." << std::endl;
            clients.emplace_back();
            LPTF_Socket& client = clients.back();
            for (LPTF_Socket& otherClient : clients) {
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

int checkArguments(int ac, char* av[]) {
    if (ac != 2) {
        std::cerr << "Usage: " << av[0] << " <port>" << std::endl;
        return false;
    }
    int port = -1;
    try {
        port = std::stoi(av[1]);
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

int main(int ac, char* av[]) {
    int port = checkArguments(ac, av);
    LPTF_Socket serverSocket;
    if (serverSocket.bindAndListen(port)) {
        std::cout << "Serveur en attente de connexions entrantes." << std::endl;
        handleClientConnections(serverSocket, port);
    } else {
        std::cerr << "Erreur lors de la liaison et de l'écoute." << std::endl;
        return 1;
    }
    return 0;
}
