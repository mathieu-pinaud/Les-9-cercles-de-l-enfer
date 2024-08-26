#ifndef LPTF_PACKET_HPP
#define LPTF_PACKET_HPP

#include <stdint.h>
#include <vector>
#include <iostream>

class LPTF_Packet {
    private :
        uint8_t packetType;
        std::vector<uint8_t> data;
        uint32_t dataSize; 
    public :
        LPTF_Packet(uint8_t packetType, std::vector<uint8_t> data) : packetType(packetType), data(data), dataSize(data.size()) {}
        LPTF_Packet(uint8_t packetType, const char* data) : packetType(packetType) {setDataFromStr(data);}
        std::vector<uint8_t> toBytes();
        LPTF_Packet fromBytes(std::vector<uint8_t> bytes);

        uint8_t getPacketType() { return packetType; }
        uint32_t getDataSize() { return dataSize; }
        std::vector<uint8_t> getData() { return data; }
        void setPacketType(const uint8_t packetType) { this->packetType = packetType; }
        void setDataSize(const uint32_t dataSize) { this->dataSize = dataSize; }
        void setData(const std::vector<uint8_t> data) { this->data = data; }
        void setDataFromStr(const char* str);
        void printPacket();
        void printData();
        void printDataSize();
        void printPacketType();
};

#endif // LPTF_PACKET_HPP