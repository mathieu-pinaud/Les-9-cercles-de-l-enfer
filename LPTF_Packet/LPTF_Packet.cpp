#include "LPTF_Packet.hpp"
#include <netinet/in.h>

std::vector<uint8_t> LPTF_Packet::toBytes() {
    std::vector<uint8_t> bytes;
    bytes.push_back(packetType);
    for (int i = 3; i >= 0; --i) {
        bytes.push_back((dataSize >> (8 * i)) & 0xFF);
    }
    bytes.insert(bytes.end(), data.begin(), data.end());
    return bytes;
}

LPTF_Packet LPTF_Packet::fromBytes(std::vector<uint8_t> bytes) {
    uint8_t packetType = bytes[0];
    uint32_t dataSize = 0;
    for (int i = 0; i < 4; ++i) {
        dataSize |= (bytes[i + 1] << (8 * (3 - i)));
    }
    std::vector<uint8_t> data(bytes.begin() + 5, bytes.end());
    return LPTF_Packet(packetType, data);
}

void LPTF_Packet::setDataFromStr(const char* str) {
    data.clear();
    for (int i = 0; str[i] != '\0'; ++i) {
        data.push_back(str[i]);
    }
    dataSize = data.size();
}

void LPTF_Packet::printPacketType() {
    std::cout << "Packet type: " << (int)packetType << std::endl;
}
void LPTF_Packet::printDataSize() {
    std::cout << "Data size: " << dataSize << std::endl;
}
void LPTF_Packet::printData() {
    std::cout << "Data: ";
    for (uint8_t byte : data) {
        std::cout << (char)byte;
    }
    std::cout << std::endl;
}
void LPTF_Packet::printPacket() {
    printPacketType();
    printDataSize();
    printData();
}