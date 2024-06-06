#include "socket_Client.hpp"

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
                if (!send(getSockFd(), message.c_str())) {
                    std::cerr << "Error sending message to server." << std::endl;
                    return false;
                }
            }
            if (FD_ISSET(getSockFd(), &readfds)) {
                std::string response = receiveMessage();
                if (!response.empty()) {
                    std::cout << std::endl << "Server response: " << response << std::endl;
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
    std::cout << "Attempting to connect to " << getAddressIp() << ":" << ntohs(getAddressPort()) << std::endl;
    if (connect(getSockFd(), (struct sockaddr *)&getAddress(), sizeof(struct sockaddr_in)) < 0) {
    perror("connection failed");
    return false;
    }
    std::cout << "Connected to server." << std::endl;
    return launchClient();
}