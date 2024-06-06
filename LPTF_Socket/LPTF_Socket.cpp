#include "LPTF_Socket.hpp"

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

std::string LPTF_Socket::receiveMessage() {
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
