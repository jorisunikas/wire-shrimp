#include "parser.hpp"
#include "parsers/ethernet_parser.hpp"
#include "parsers/http_parser.hpp"
#include "parsers/ipv4_parser.hpp"
#include "parsers/ipv6_parser.hpp"
#include "parsers/tcp_parser.hpp"
#include "parsers/udp_parser.hpp"

ParsedPacket Parser::parse(RawPacket rp) {
    EthernetParser ethernetParser;
    IPv4Parser ipv4Parser;
    IPv6Parser ipv6Parser;
    TCPParser tcpParser;
    UDPParser udpParser;
    HTTPParser httpParser;
    ParsedPacket pp;
    int offset = 0;

    // ===== LAYER 2: ETHERNET =====
    if (ethernetParser.isValid(rp)) {
        pp.valid = true;
        pp.ethData = ethernetParser.parse(rp);
    } else {
        pp.valid = false;
        return pp; // Invalid packet (too short for Ethernet header)
    }

    // ===== LAYER 3: IPv4, IPv6 =====

    int l3Type = pp.ethData.etherType;
    if (l3Type == 0x0800 && ipv4Parser.isValid(rp)) {
        pp.IPv4Data = ipv4Parser.parse(rp);
        pp.protocol.append("IPv4");
    } else if (l3Type == 0x86DD && ipv6Parser.isValid(rp)) {
        pp.protocol.append("IPv6");
        return pp;
    } else if (l3Type == 0x0806) {
        pp.protocol.append("ARP");
        return pp;
    } else {
        return pp;
    }

    // ===== LAYER 4: TCP/UDP =====

    /*
     * Parse transport layer based on IPv4 protocol field
     * 1 = ICMP, 6 = TCP, 17 = UDP
     */

    // TCP
    if (pp.IPv4Data->protocol == 6) {

        if (tcpParser.isValid(rp)) {
            pp.tcpData = tcpParser.parse(rp);
            pp.protocol.append(" TCP");
        }

        // HTTP
        if (pp.tcpData->srcPort == 80 || pp.tcpData->dstPort == 80) {
            if (httpParser.isValid(rp)) {
                pp.protocol.append(" HTTP");
                pp.httpData = httpParser.parse(rp);
            }
        }
    }
    // UDP
    else if (pp.IPv4Data->protocol == 17) {
        if (udpParser.isValid(rp)) {
            pp.udpData = udpParser.parse(rp);
            pp.protocol.append(" UDP");
        }
    }
    // ICMP (no parsing implemented)
    else if (pp.IPv4Data->protocol == 1) {

        pp.protocol.append(" ICMP");
    }

    // ===== LAYER 6: HTTP/HTTPS/DNS =====

    return pp;
}
