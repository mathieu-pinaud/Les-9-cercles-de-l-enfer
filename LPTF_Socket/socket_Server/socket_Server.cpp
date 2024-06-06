#include "socket_Server.hpp"


bool socket_Server::initServer() {
    setSockFd(socket(AF_INET, SOCK_STREAM, 0));
    if (getSockFd() == 0) {
        perror("socket creation failed");
        return false;
    }
    displayServerAddress();
    setAddressFamily(AF_INET);
    setAddressAdr(INADDR_ANY);
    setAddressPort(htons(getPort()));    
    if (bind(getSockFd(), (struct sockaddr *)&getAddress(), sizeof(getAddress())) < 0) {
        perror("bind failed");
        return false;
    }
    if (listen(getSockFd(), 3) < 0) {
        perror("listen");
        return false;
    }
    return true;
}

int socket_Server::acceptConnection() {
    int addrlen = sizeof(getAddress());
    int new_socket;
    if ((new_socket = accept(getSockFd(), (struct sockaddr *)&getAddress(), (socklen_t*)&addrlen)) < 0) {
        std::cout << "error accepting connection" << std::endl;
        return -1;
    }
    return new_socket;
}

void socket_Server::displayServerAddress() {
    struct ifaddrs *ifAddrStruct = NULL;
    struct ifaddrs *ifa = NULL;
    void *tmpAddrPtr = NULL;

    getifaddrs(&ifAddrStruct);

    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) {
            continue;
        }
        // check it is IP4 and not a loopback address
        if (ifa->ifa_addr->sa_family == AF_INET && strcmp(ifa->ifa_name, "lo") != 0) { 
            tmpAddrPtr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            std::cout << "Adresse IP du serveur : " << addressBuffer << std::endl;
            break;
        }
    }
    if (ifAddrStruct != NULL) {
        freeifaddrs(ifAddrStruct);
    }
}

void socket_Server::handleNewConnection(fd_set& readfds) {
    if (FD_ISSET(getSockFd(), &readfds)) {
        int new_socket = acceptConnection();
        if (new_socket < 0) {
            return;
        }
        getClientSockets().push_back(new_socket);
        std::cout << "Nouvelle connexion, socket fd : "
        << new_socket << std::endl;
    }
}

void socket_Server::removeClient(int clientSocket) {
    close(clientSocket);
    auto it = std::find(getClientSockets().begin(), getClientSockets().end(), clientSocket);
        getClientSockets().erase(it);
    if (it != getClientSockets().end()) {
         getClientSockets().erase(it);
    }
}

void socket_Server::handleClientCommunication(int clientSocket) {
    char buffer[2040];
    int bytesReceived = recv(clientSocket, buffer, 2040, 0);
    if (bytesReceived <= 0) {
        if (bytesReceived == 0) {
            std::cout << "Client disconnected. Socket FD: "
            << clientSocket << std::endl;
        } else {
            perror("recv");
        }
        removeClient(clientSocket);
    } else {
        buffer[bytesReceived] = '\0';
        std::cout << "Received from client " << clientSocket
        << ": " << buffer << std::endl;
        for (int otherClientSocket : getClientSockets()) {
            if (otherClientSocket != clientSocket) {
                send(otherClientSocket, buffer);
            }
        }
    }
}


void socket_Server::launchServer() {
    while (true) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(getSockFd(), &readfds);
        int max_fd = getSockFd();
        for (int clientSocket : getClientSockets()) {
            FD_SET(clientSocket, &readfds);
            max_fd = std::max(max_fd, clientSocket);
        }
        int activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);
        if (activity < 0) {
            perror("select");
            break;
        }
        handleNewConnection(readfds);
        for (int clientSocket : getClientSockets()) {
            if (FD_ISSET(clientSocket, &readfds)) {
                handleClientCommunication(clientSocket);
            }
        }
    }
}