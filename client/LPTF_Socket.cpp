#include "LPTF_Socket.hpp"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <string.h>

LPTF_Socket::LPTF_Socket() : sockfd(socket(AF_INET, SOCK_STREAM, 0)) {
    if (sockfd < 0) {
        std::cerr << "Erreur lors de la création du socket" << std::endl;
    }
}

LPTF_Socket::~LPTF_Socket() {
    close(sockfd);
}

bool LPTF_Socket::connectTo(const char* address, int port) {
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, address, &serv_addr.sin_addr) <= 0) {
        std::cerr << "Adresse invalide" << std::endl;
        return false;
    }
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Échec de la connexion" << std::endl;
        return false;
    }
    return true;
}

bool LPTF_Socket::sendMessage(const char* message) {
    if (send(sockfd, message, std::strlen(message), 0) < 0) {
        std::cerr << "Erreur lors de l'envoi du message" << std::endl;
        return false;
    }
    return true;
}

std::string LPTF_Socket::receiveMessage() {
    char buffer[1024];
    int bytesRead = recv(sockfd, buffer, sizeof(buffer), 0);
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0'; // Ajouter le caractère de fin de chaîne
        return std::string(buffer);
    } else {
        std::cerr << "Erreur lors de la réception du message" << std::endl;
        return "";
    }
}
