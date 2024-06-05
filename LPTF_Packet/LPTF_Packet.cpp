#include "LPTF_Packet.hpp"
#include <netinet/in.h>

std::vector<uint8_t> LPTF_Packet::toBytes(uint8_t packetType, std::vector<uint8_t> data) {
    std::vector<uint8_t> bytes;
    bytes.push_back(htonl(packetType));

}

LPTF_Packet LPTF_Packet::fromBytes(std::vector<uint8_t> bytes) {
    uint8_t packetType = bytes[0];
    std::vector<uint8_t> data(bytes.begin() + 1, bytes.end());
    return LPTF_Packet(packetType, data);
}