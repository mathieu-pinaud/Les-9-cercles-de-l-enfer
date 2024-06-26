#include "../LPTF_Socket/socket_Server/socket_Server.hpp"
#include "../window/window.hpp"
#include <vector>




int main(int ac, char* av[]) {
    WindowQT windowQT;
    windowQT.windows(ac, av);
    int port = checkArgs(ac, av);
    if (port == -1)
        return 1;
    socket_Server serverSocket(port);
    if (!serverSocket.initServer()) {
        std::cerr << "Failed to initialize server." << std::endl;
        return 1;
    }
    serverSocket.launchServer();
}