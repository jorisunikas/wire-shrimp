#include "tcp_parser.hpp"

#include <algorithm>
#include <iostream>
#include <vector>


// Reads 16-bit port number from the given data pointer
uint16_t readPort(const uint8_t* data) {
    return (data[0] << 8) | data[1];
}

TCPHeader* TCPParser::parse(RawPacket rp) {
    TCPHeader* header = new TCPHeader();
    const uint8_t *data = rp.data + ETHERNET_HEADER_SIZE + IPV4_MIN_HEADER_SIZE; // TCP header starts after Ethernet + IPv4 headers
    size_t len = rp.len - ETHERNET_HEADER_SIZE - IPV4_MIN_HEADER_SIZE;

    header->srcPort = readPort(data);
    header->dstPort = readPort(data + 2);
    header->flags = data[13];

    return header;
}

bool TCPParser::isValid(RawPacket rp) {
    if (rp.len < TCP_MIN_HEADER_SIZE) {
        return false; // Not enough data for minimum TCP header
    }
    // Additional checks can be added here (e.g. valid ports, flags)
    return true;
}