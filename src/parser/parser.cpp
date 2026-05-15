#include "parser.hpp"
#include "../headers.hpp"
#include "../packet.hpp"
#include <cstddef>
#include <cstdint>

using namespace std;

static constexpr uint8_t ETHERNET_HEADER_SIZE = 14;
static constexpr uint8_t IPV4_MIN_HEADER_SIZE = 20;

// Static parser instances
unique_ptr<EthernetParser> Parser::ethernetParser = nullptr;
unique_ptr<IPv4Parser> Parser::ipv4Parser = nullptr;
unique_ptr<TCPParser> Parser::tcpParser = nullptr;
unique_ptr<UDPParser> Parser::udpParser = nullptr;

void Parser::initializeParsers() {
    if (ethernetParser == nullptr) {
        ethernetParser = make_unique<EthernetParser>();
        ipv4Parser = make_unique<IPv4Parser>();
        tcpParser = make_unique<TCPParser>();
        udpParser = make_unique<UDPParser>();
    }
}

ParsedPacket Parser::parse(const uint8_t *data, size_t len) {
    ParsedPacket pp;
    int offset = 0;

    // Initialize parsers on first use
    initializeParsers();

    /*
     * Parse Ethernet header (first 14 bytes)
     * If packet is less than min length, then it is invalid
     */
    if (!ethernetParser->parse(data, len)) {
        return pp;
    }
    pp.eth = ethernetParser->getHeader();
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
    if (!ipv4Parser->parse(data + offset, len - offset)) {
        return pp;
    }
    pp.ip = ipv4Parser->getHeader();
    offset += ipv4Parser->getHeaderSize();

    /*
     * Parse transport layer based on IPv4 protocol field
     * 1 = ICMP, 6 = TCP, 17 = UDP
     */
    if (pp.ip->protocol == 6) {
        // TCP
        if (tcpParser->parse(data + offset, len - offset)) {
            pp.tcp = tcpParser->getHeader();
            pp.protocol.append(" TCP");
        }
    } else if (pp.ip->protocol == 17) {
        // UDP
        if (udpParser->parse(data + offset, len - offset)) {
            pp.udp = udpParser->getHeader();
            pp.protocol.append(" UDP");
        }
    } else if (pp.ip->protocol == 1) {
        // ICMP (no parsing implemented)
        pp.protocol.append(" ICMP");
    }

    return pp;
}
