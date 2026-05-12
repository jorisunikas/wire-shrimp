#include "parser.hpp"
#include "../headers.hpp"
#include "../packet.hpp"
#include <cstddef>
#include <cstdint>

static constexpr uint8_t ETHERNET_HEADER_SIZE = 14;
static constexpr uint8_t IPV4_MIN_HEADER_SIZE = 20;

// Static strategy instances
std::unique_ptr<EthernetStrategy> Parser::ethernetStrategy = nullptr;
std::unique_ptr<IPv4Strategy> Parser::ipv4Strategy = nullptr;
std::unique_ptr<TCPStrategy> Parser::tcpStrategy = nullptr;
std::unique_ptr<UDPStrategy> Parser::udpStrategy = nullptr;

void Parser::initializeStrategies() {
    if (ethernetStrategy == nullptr) {
        ethernetStrategy = std::make_unique<EthernetStrategy>();
        ipv4Strategy = std::make_unique<IPv4Strategy>();
        tcpStrategy = std::make_unique<TCPStrategy>();
        udpStrategy = std::make_unique<UDPStrategy>();
    }
}

ParsedPacket Parser::parse(const uint8_t *data, size_t len) {
    ParsedPacket pp;
    int offset = 0;

    // Initialize strategies on first use
    initializeStrategies();

    /*
     * Parse Ethernet header (first 14 bytes)
     * If packet is less than min length, then it is invalid
     */
    if (!ethernetStrategy->parse(data, len)) {
        return pp;
    }
    pp.eth = ethernetStrategy->getHeader();
    offset += ETHERNET_HEADER_SIZE;

    /*
     * Determine protocol type from EtherType field
     * If packets are IPv6 or ARP, return them without parsing inner headers
     */
    if (pp.eth.etherType == 0x86DD) {
        pp.valid = true;
        pp.protocol = "IPv6";
        return pp;
    } else if (pp.eth.etherType == 0x0806) {
        pp.valid = true;
        pp.protocol = "ARP";
        return pp;
    } else if (pp.eth.etherType != 0x0800) {
        pp.protocol = "Other";
        pp.valid = true;
        return pp;
    }

    pp.protocol = "IPv4";
    pp.valid = true;

    /*
     * Parse IPv4 header
     * If packet is less than min length, then it is invalid.
     * EthernetHeader + IPv4Header = 34 bytes
     */
    if (!ipv4Strategy->parse(data + offset, len - offset)) {
        return pp;
    }
    pp.ip = ipv4Strategy->getHeader();
    offset += ipv4Strategy->getHeaderSize();

    /*
     * Parse transport layer based on IPv4 protocol field
     * 1 = ICMP, 6 = TCP, 17 = UDP
     */
    if (pp.ip->protocol == 6) {
        // TCP
        if (tcpStrategy->parse(data + offset, len - offset)) {
            pp.tcp = tcpStrategy->getHeader();
            pp.protocol.append(" TCP");
        }
    } else if (pp.ip->protocol == 17) {
        // UDP
        if (udpStrategy->parse(data + offset, len - offset)) {
            pp.udp = udpStrategy->getHeader();
            pp.protocol.append(" UDP");
        }
    } else if (pp.ip->protocol == 1) {
        // ICMP (no parsing implemented)
        pp.protocol.append(" ICMP");
    }

    return pp;
}
