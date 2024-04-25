#include "LPTF_Socket.hpp"

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
    std::cout << socket_fd << (struct sockaddr *)&address << std::endl;
    if (connect(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("connection failed");
        return false;
    }
    return true;
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
        perror("accept");
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

ssize_t LPTF_Socket::receive(char* buffer, int buffer_size) {
    ssize_t valread;
    if ((valread = read(socket_fd, buffer, buffer_size)) < 0) {
        perror("read");
    }
    return valread;
}

void LPTF_Socket::closeSocket() {
    close(socket_fd);
}

void LPTF_Socket::displayServerAddress() {
    char hostname[128];
    gethostname(hostname, sizeof(hostname));
    struct hostent* host_entry = gethostbyname(hostname);
    char* ip = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));
    ipAddr = inet_addr(ip);
    std::cout << "Adresse IP du serveur : " << ip << std::endl;
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
        std::string response = "Message received successfully!";
        for (int clientSocket : clientSockets) {
            if (clientSocket != clientSocket) {
                send(clientSocket, response.c_str());
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
            handleClientCommunication(clientSocket);
        }
    }
}
