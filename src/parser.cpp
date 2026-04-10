#include "parser.hpp"
#include "headers.hpp"
#include "packet.hpp"
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <sstream>

static constexpr uint8_t ETHERNET_HEADER_SIZE = 14;
static constexpr uint8_t IPV4_MIN_HEADER_SIZE = 20;

static std::string readMac(const uint8_t *data) {
    std::ostringstream oss;
    for (int i = 0; i < 6; i++) {
        if (i > 0)
            oss << ':';
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)data[i];
    }
    return oss.str();
}

static std::string readIPv4Address(const uint8_t *data) {
    std::ostringstream oss;
    for (int i = 0; i < 4; i++) {
        if (i > 0)
            oss << '.';
        oss << (int)data[i];
    }
    return oss.str();
}

EthernetHeader Parser::parseEthernet(const uint8_t *data) {
    EthernetHeader eh;
    eh.dstMac = readMac(data);
    eh.srcMac = readMac(data + 6);
    eh.etherType = (data[12] << 8) | data[13];
    return eh;
}

IPv4Header Parser::parseIPv4(const uint8_t *data) {
    IPv4Header ip;
    ip.ihl = data[0] & 0xF;
    ip.ttl = data[8];
    ip.protocol = data[9];
    ip.srcIp = readIPv4Address(data + 12);
    ip.dstIp = readIPv4Address(data + 16);
    return ip;
}

TCPHeader Parser::parseTCP(const uint8_t *data) {
    TCPHeader tcp;
    tcp.srcPort = (data[0] << 8) | data[1];
    tcp.dstPort = (data[2] << 8) | data[3];
    tcp.flags = data[13];
    return tcp;
}

UDPHeader Parser::parseUDP(const uint8_t *data) {
    UDPHeader udp;
    udp.srcPort = (data[0] << 8) | data[1];
    udp.dstPort = (data[2] << 8) | data[3];
    return udp;
}

ParsedPacket Parser::parse(const uint8_t *data, std::size_t len) {
    ParsedPacket pp;
    int offset = 0;

    /*
     * If packet is less than min length, then it is invalid
     * EthernetHeader = 14
     */

    if (len < ETHERNET_HEADER_SIZE) {
        return pp;
    }
    pp.eth = parseEthernet(data);
    offset += ETHERNET_HEADER_SIZE;

    /* If packets are IPv6 or ARP return them, no parsing implemented yet */
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
     * If packet is less than min length, then it is invalid.
     * EthernetHeader + IPv4Header = 34
     */
    if (len < IPV4_MIN_HEADER_SIZE + ETHERNET_HEADER_SIZE) {
        return pp;
    }
    pp.ip = parseIPv4(data + offset);
    offset += pp.ip->ihl * 4;

    /* If packets are ICMP, do not parse them, just set protocol string */
    if (pp.ip->protocol == 6) {
        pp.tcp = parseTCP(data + offset);
        pp.protocol.append(" TCP");
    } else if (pp.ip->protocol == 17) {
        pp.udp = parseUDP(data + offset);
        pp.protocol.append(" UDP");
    } else if (pp.ip->protocol == 1) {
        pp.protocol.append(" ICMP");
    }

    return pp;
}
