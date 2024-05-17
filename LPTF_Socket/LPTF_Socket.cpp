#include "LPTF_Socket.hpp"

bool LPTF_Socket::launchClient() {
    fd_set readfds;
    int max_fd = std::max(STDIN_FILENO, socket_fd) + 1;

    while (true) {
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        FD_SET(socket_fd, &readfds);

        int ret = select(max_fd, &readfds, NULL, NULL, NULL);
        if (ret > 0) {
            if (FD_ISSET(STDIN_FILENO, &readfds)) {
                std::string message;
                std::getline(std::cin, message);
                if (message == "stop") {
                    std::cout << "Closing connection..." << std::endl;
                    break;
                }
                if (!send(socket_fd, message.c_str())) {
                    std::cerr << "Error sending message to server." << std::endl;
                    return false;
                }
            }
            if (FD_ISSET(socket_fd, &readfds)) {
                std::string response = receiveClient();
                if (!response.empty()) {
                    std::cout << std::endl << "Server response: " << response << std::endl;
                }
            }
        }
    }
    return true;
}

bool LPTF_Socket::initClient(const char* server_ip) {
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
        return false;
    }
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    if(inet_pton(AF_INET, server_ip, &address.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return false;
    }
    std::cout << "Attempting to connect to " << inet_ntoa(address.sin_addr) << ":" << ntohs(address.sin_port) << std::endl;
    if (connect(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("connection failed");
        return false;
    }
    std::cout << "Connected to server." << std::endl;
    return launchClient();

}

bool LPTF_Socket::initServer() {
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket creation failed");
        return false;
    }
    displayServerAddress();
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);    
    if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return false;
    }
    if (listen(socket_fd, 3) < 0) {
        perror("listen");
        return false;
    }
    return true;
}

int LPTF_Socket::acceptConnection() {
    int addrlen = sizeof(address);
    int new_socket;
    if ((new_socket = accept(socket_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        std::cout << "error accepting connection" << std::endl;
        return -1;
    }
    return new_socket;
}

bool LPTF_Socket::send(int socket_fd, const char* data) {
    if (::send(socket_fd, data, strlen(data), 0) < 0) {
        perror("send");
        return false;
    }
    return true;
}

void LPTF_Socket::closeSocket() {
    close(socket_fd);
}

void LPTF_Socket::displayServerAddress() {
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

void LPTF_Socket::handleNewConnection(fd_set& readfds) {
    if (FD_ISSET(socket_fd, &readfds)) {
        int new_socket = acceptConnection();
        if (new_socket < 0) {
            return;
        }
        clientSockets.push_back(new_socket);
        std::cout << "Nouvelle connexion, socket fd : "
        << new_socket << std::endl;
    }
}

void LPTF_Socket::removeClient(int clientSocket) {
    close(clientSocket);
    auto it = std::find(clientSockets.begin(), clientSockets.end(), clientSocket);
        clientSockets.erase(it);
    if (it != clientSockets.end()) {
         clientSockets.erase(it);
    }
}

void LPTF_Socket::handleClientCommunication(int clientSocket) {
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
        for (int otherClientSocket : clientSockets) {
            if (otherClientSocket != clientSocket) {
                send(otherClientSocket, buffer);
            }
        }
    }
}


void LPTF_Socket::launchServer() {
    while (true) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(socket_fd, &readfds);
        int max_fd = socket_fd;
        for (int clientSocket : clientSockets) {
            FD_SET(clientSocket, &readfds);
            max_fd = std::max(max_fd, clientSocket);
        }
        int activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);
        if (activity < 0) {
            perror("select");
            break;
        }
        handleNewConnection(readfds);
        for (int clientSocket : clientSockets) {
            if (FD_ISSET(clientSocket, &readfds)) {
                handleClientCommunication(clientSocket);
            }
        }
    }
}

std::string LPTF_Socket::receiveClient() {
    char buffer[2040];
    int bytesReceived = recv(socket_fd, buffer, 2040, 0);
    if (bytesReceived <= 0) {
        if (bytesReceived == 0) {
            std::cout << "Server disconnected." << std::endl;
        } else {
            perror("recv");
        }
        return "";
    }
    buffer[bytesReceived] = '\0';
    return std::string(buffer);
}
