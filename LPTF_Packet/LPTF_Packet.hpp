#ifndef LPTF_PACKET_HPP
#define LPTF_PACKET_HPP

#include <stdint.h>
#include <vector>

class LPTF_Packet {
    public : 
        uint8_t packetType;
        std::vector<uint8_t> data;

        LPTF_Packet(uint8_t packetType, std::vector<uint8_t> data) : packetType(packetType), data(data) {}
        std::vector<uint8_t> toBytes(uint8_t packetType, std::vector<uint8_t> data);
        LPTF_Packet fromBytes(std::vector<uint8_t> bytes);
};

#endif // LPTF_PACKET_HPP