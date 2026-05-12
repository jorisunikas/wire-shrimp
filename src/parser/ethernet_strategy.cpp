#include "ethernet_strategy.hpp"
#include <iomanip>
#include <sstream>

static constexpr uint8_t ETHERNET_HEADER_SIZE = 14;

static std::string readMac(const uint8_t *data) {
    std::ostringstream oss;
    for (int i = 0; i < 6; i++) {
        if (i > 0)
            oss << ':';
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)data[i];
    }
    return oss.str();
}

bool EthernetStrategy::parse(const uint8_t *data, size_t len) {
    if (len < ETHERNET_HEADER_SIZE) {
        return false;
    }

    header.dstMac = readMac(data);
    header.srcMac = readMac(data + 6);
    header.etherType = (data[12] << 8) | data[13];
    
    return true;
}

EthernetHeader EthernetStrategy::getHeader() const {
    return header;
}
