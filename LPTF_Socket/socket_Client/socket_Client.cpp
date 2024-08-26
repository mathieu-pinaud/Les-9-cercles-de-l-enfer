#include "socket_Client.hpp"

bool socket_Client::receive(int sockfd, LPTF_Packet& packet) {
    char buffer[1024];
    int bytesRead = read(sockfd, buffer, sizeof(buffer));
    if (bytesRead <= 0) {
        return false;
    }
    packet.setDataFromStr(buffer);
    return true;
}

bool socket_Client::launchClient() {
    fd_set readfds;
    int max_fd = std::max(STDIN_FILENO, getSockFd()) + 1;

    while (true) {
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        FD_SET(getSockFd(), &readfds);

        int ret = select(max_fd, &readfds, NULL, NULL, NULL);
        if (ret > 0) {
            if (FD_ISSET(STDIN_FILENO, &readfds)) {
                std::string message;
                std::getline(std::cin, message);
                if (message == "stop") {
                    std::cout << "Closing connection..." << std::endl;
                    break;
                }
                if (!send(getSockFd(), LPTF_Packet(0, message.c_str()))) {
                    std::cerr << "Error sending message to server." << std::endl;
                    return false;
                }
            }
            if (FD_ISSET(getSockFd(), &readfds)) {
                LPTF_Packet responsePacket = LPTF_Packet(0, "");
                if (!receive(getSockFd(), responsePacket)) {
                    std::cerr << "Error receiving message from server." << std::endl;
                    return false;
                }
                if (!responsePacket.getData().empty()) {
                    responsePacket.printPacket();
                }
            }
        }
    }
    return true;
}

bool socket_Client::initClient(const char* server_ip) {
    setSockFd(socket(AF_INET, SOCK_STREAM, 0));
    if (getSockFd() < 0) {
        perror("socket creation failed");
        return false;
    }
    setAddressFamily(AF_INET);
    setAddressPort(htons(getPort()));
    if(!setAddressIp(server_ip)) {
        perror("Invalid address/ Address not supported");
        return false;
    }
    std::cout << "Attempting to connect to " << getAddressIp() << ":" << getAddressPort() << std::endl;
    if (connect(getSockFd(), (struct sockaddr *)&getAddress(), sizeof(struct sockaddr_in)) < 0) {
    perror("connection failed");
    return false;
    }
    std::cout << "Connected to server." << std::endl;
    return launchClient();
}