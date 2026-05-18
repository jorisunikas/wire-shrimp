#include "ipv6_parser.hpp"

#include <sstream>

static std::string readIPv6Address(const uint8_t *data) {
    std::ostringstream oss;
    for (int i = 0; i < 8; i++) {
        if (i > 0)
            oss << ':';
        oss << std::hex << ((data[i] << 8) | data[i + 1]);
    }
    return oss.str();
}

IPv6Header IPv6Parser::parse(RawPacket& rp) {
    IPv6Header header;
    const uint8_t *data = rp.data + ETHERNET_HEADER_SIZE;

    header.payloadLength = (data[4] << 8) | data[5];
    header.protocol = data[6];
    header.hopLimit = data[7];
    header.srcIp = readIPv6Address(data + 8);
    header.dstIp = readIPv6Address(data + 24);

    return header;
}

bool IPv6Parser::isValid(RawPacket& rp) {
    if (rp.len < ETHERNET_HEADER_SIZE + IPV6_MIN_HEADER_SIZE) {
        return false;
    }
    uint8_t version = rp.data[ETHERNET_HEADER_SIZE] >> 4;
    return version == 6;
}
