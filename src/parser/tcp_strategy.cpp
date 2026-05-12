#include "tcp_strategy.hpp"

static constexpr uint8_t TCP_MIN_HEADER_SIZE = 20;

bool TCPStrategy::parse(const uint8_t *data, size_t len) {
    if (len < TCP_MIN_HEADER_SIZE) {
        return false;
    }

    header.srcPort = (data[0] << 8) | data[1];
    header.dstPort = (data[2] << 8) | data[3];
    header.flags = data[13];
    
    return true;
}

TCPHeader TCPStrategy::getHeader() const {
    return header;
}
