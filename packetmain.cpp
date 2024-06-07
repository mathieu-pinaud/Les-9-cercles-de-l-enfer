#include "./LPTF_Packet/LPTF_Packet.hpp"
#include <iostream>

int main(int argc, char* av[]){
    LPTF_Packet packet(1, "Hello, world!");
    std::vector<uint8_t> bytes = packet.toBytes();
    LPTF_Packet packet2 = packet.fromBytes(bytes);
    packet2.printPacket();
    return 0;
}