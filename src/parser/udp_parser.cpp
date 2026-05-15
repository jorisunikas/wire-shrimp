#include "udp_parser.hpp"

UDPHeader* UDPParser::parse(RawPacket rp) {
    UDPHeader* header = new UDPHeader();
    const uint8_t *data = rp.data + ETHERNET_HEADER_SIZE + IPV4_MIN_HEADER_SIZE; // UDP header starts after Ethernet + IPv4 headers
    size_t len = rp.len;

    header->srcPort = (data[0] << 8) | data[1];
    header->dstPort = (data[2] << 8) | data[3];
    header->length = (data[4] << 8) | data[5];
    header->checksum = (data[6] << 8) | data[7];

    return header;
}

bool UDPParser::isValid(RawPacket rp) {
    if (rp.len < UDP_MIN_HEADER_SIZE) {
        return false; // Not enough data for minimum UDP header
    }
    // Additional checks can be added here (e.g. valid ports)
    return true;
}
