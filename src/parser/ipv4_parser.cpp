#include "ipv4_parser.hpp"
#include <sstream>

static std::string readIPv4Address(const uint8_t* data) {
    std::ostringstream oss;
    for (int i = 0; i < 4; i++) {
        if (i > 0)
            oss << '.';
        oss << (int)data[i];
    }
    return oss.str();
}

IPv4Header* IPv4Parser::parse(RawPacket rp) {
    IPv4Header* header = new IPv4Header();
    const uint8_t *data = rp.data + ETHERNET_HEADER_SIZE;


    header->dstIp = data[0] & 0xF;
    header->ttl = data[8];
    header->protocol = data[9];
    header->srcIp = readIPv4Address(data + 12);
    header->dstIp = readIPv4Address(data + 16);
    
    return header;
}

bool IPv4Parser::isValid(RawPacket rp) {
    if (rp.len < ETHERNET_HEADER_SIZE + IPV4_MIN_HEADER_SIZE) {
        return false; // Not enough data for Ethernet + minimum IPv4 header
    }
    const uint8_t *data = rp.data + ETHERNET_HEADER_SIZE;
    uint8_t version = data[0] >> 4;
    return version == 4; // Check if version field is 4 for IPv4
}
