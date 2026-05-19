#include "ipv6_parser.hpp"

#include <cstdint>
#include <sstream>

static std::string readIPv6Address(const uint8_t *data) {
    std::ostringstream oss;
    for (int i = 0; i < 16; i += 2) {
        if (i > 0)
            oss << ':';
        oss << std::hex << ((data[i] << 8) | data[i + 1]);
    }
    return oss.str();
}

IPv6Header IPv6Parser::parse(RawPacket &rp) {
    IPv6Header header;
    const uint8_t *data = rp.data + ETHERNET_HEADER_SIZE;

    header.payloadLength = (data[4] << 8) | data[5];
    header.protocol = data[6];
    header.hopLimit = data[7];
    header.srcIp = readIPv6Address(data + 8);
    header.dstIp = readIPv6Address(data + 24);

    // IPv6 might contain extension headers, need to skip them to access UDP/TCP
    int offset = 40;
    uint8_t nextHeader = header.protocol;

    while (nextHeader == 0 || nextHeader == 43 || nextHeader == 44 ||
           nextHeader == 60) {

        if (ETHERNET_HEADER_SIZE + offset + 2 > rp.len)
            break;

        const uint8_t *ext = rp.data + ETHERNET_HEADER_SIZE + offset;

        if (nextHeader == 44) {
            // Fragment header is always 8 bytes, no length field
            nextHeader = ext[0];
            offset += 8;
        } else {
            nextHeader = ext[0];
            // first byte contains the length
            offset += (ext[1] + 1) * 8;
        }
    }

    header.protocol = nextHeader;

    return header;
}

bool IPv6Parser::isValid(RawPacket &rp) {
    if (rp.len < ETHERNET_HEADER_SIZE + IPV6_MIN_HEADER_SIZE) {
        return false;
    }
    uint8_t version = rp.data[ETHERNET_HEADER_SIZE] >> 4;
    return version == 6;
}
