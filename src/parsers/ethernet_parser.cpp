#include "ethernet_parser.hpp"

#include <iomanip>
#include <sstream>

static std::string readMac(RawPacket rp) {
    std::ostringstream oss;
    for (int i = 0; i < 6; i++) {
        if (i > 0)
            oss << ':';
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)rp.data[i];
    }
    return oss.str();
}

EthernetHeader EthernetParser::parse(RawPacket rp) {
    EthernetHeader header;
    header.dstMac = readMac(rp);
    header.srcMac = readMac(rp);
    header.etherType = (rp.data[12] << 8) | rp.data[13];

    if(header.etherType == 0x0800) {
        header.etherTypeStr = "IPv4";
    } else if (header.etherType == 0x0806) {
        header.etherTypeStr = "ARP";
    } else if (header.etherType == 0x86DD) {
        header.etherTypeStr = "IPv6";
    } else {
        header.etherTypeStr = "Other";
    }

    return header;
}

bool EthernetParser::isValid(RawPacket rp){
    return rp.len >= ETHERNET_HEADER_SIZE;
}
