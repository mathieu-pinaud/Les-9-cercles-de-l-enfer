#include "LPTF_Socket.hpp"

bool LPTF_Socket::send(int socket_fd, LPTF_Packet packet) {
    std::vector<uint8_t> bytes = packet.toBytes();
    if (::send(socket_fd, bytes.data() , bytes.size(), 0) <0) {
        perror("send");
        return false;
    }
    return true;
}

void LPTF_Socket::closeSocket() {
    close(socket_fd);
}

LPTF_Packet LPTF_Socket::receivePacket() {
    char buffer[2040];
    int bytesReceived = recv(socket_fd, buffer, 2040, 0);
    if (bytesReceived <= 0) {
        if (bytesReceived == 0) {
            std::cout << "Server disconnected." << std::endl;
            exit(0);
        } else {
            perror("recv");
        }
        return "";
    }
    buffer[bytesReceived] = '\0';
    return std::string(buffer);
}
