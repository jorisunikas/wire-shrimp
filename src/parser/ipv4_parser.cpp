#include "ipv4_parser.hpp"
#include <sstream>

static constexpr uint8_t IPV4_MIN_HEADER_SIZE = 20;

static std::string readIPv4Address(const uint8_t *data) {
    std::ostringstream oss;
    for (int i = 0; i < 4; i++) {
        if (i > 0)
            oss << '.';
        oss << (int)data[i];
    }
    return oss.str();
}

bool IPv4Parser::parse(const uint8_t *data, size_t len) {
    if (len < IPV4_MIN_HEADER_SIZE) {
        return false;
    }

    header.ihl = data[0] & 0xF;
    header.ttl = data[8];
    header.protocol = data[9];
    header.srcIp = readIPv4Address(data + 12);
    header.dstIp = readIPv4Address(data + 16);
    
    return true;
}

IPv4Header IPv4Parser::getHeader() const {
    return header;
}

uint8_t IPv4Parser::getHeaderSize() const {
    return header.ihl * 4;
}
