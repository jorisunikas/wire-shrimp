#include "udp_strategy.hpp"

static constexpr uint8_t UDP_MIN_HEADER_SIZE = 8;

bool UDPStrategy::parse(const uint8_t *data, size_t len) {
    if (len < UDP_MIN_HEADER_SIZE) {
        return false;
    }

    header.srcPort = (data[0] << 8) | data[1];
    header.dstPort = (data[2] << 8) | data[3];
    
    return true;
}

UDPHeader UDPStrategy::getHeader() const {
    return header;
}
