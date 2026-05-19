#include "parser.hpp"
#include "indexer.hpp"
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
    Indexer indexer;

    int offset = 0;

    // Initialize parsers on first use
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
    int l4Type = -1;

    if (l3Type == 0x0800 && ipv4Parser.isValid(rp)) {
        pp.IPv4Data = ipv4Parser.parse(rp);
        pp.protocol.append("IPv4");
        l4Type = pp.IPv4Data->protocol;
    } else if (l3Type == 0x86DD && ipv6Parser.isValid(rp)) {
        pp.IPv6Data = ipv6Parser.parse(rp);
        pp.protocol.append("IPv6");
        l4Type = pp.IPv6Data->protocol;
    } else if (l3Type == 0x0806) {
        pp.protocol.append("ARP");
        return pp;
    } else {
        return pp;
    }

    // ===== LAYER 4: TCP/UDP =====

    if (l4Type == 6) {
        if (tcpParser.isValid(rp)) {
            pp.tcpData = tcpParser.parse(rp);
            pp.protocol.append(" TCP");

            if (pp.tcpData->srcPort == 80 || pp.tcpData->dstPort == 80) {
                if (httpParser.isValid(rp)) {
                    pp.protocol.append(" HTTP");
                    pp.httpData = httpParser.parse(rp, indexer);
                }

                std::string srcIp =
                    pp.IPv4Data ? pp.IPv4Data->srcIp : pp.IPv6Data->srcIp;
                std::string dstIp =
                    pp.IPv4Data ? pp.IPv4Data->dstIp : pp.IPv6Data->dstIp;

                if (indexer.getURL(srcIp) != "") {
                    pp.httpData->hostURL = indexer.getURL(srcIp);
                } else if (pp.httpData) {
                    pp.httpData->hostURL =
                        httpParser.extractHostURL(*pp.httpData);
                    if (pp.httpData->hostURL != "") {
                        indexer.addURL(dstIp, pp.httpData->hostURL);
                    }
                }
            }
        }
    } else if (l4Type == 17) {
        if (udpParser.isValid(rp)) {
            pp.udpData = udpParser.parse(rp);
            pp.protocol.append(" UDP");
        }
    } else if (l4Type == 1) {
        pp.protocol.append(" ICMP");
    } else if (l4Type == 58) {
        pp.protocol.append(" ICMPv6");
    }

    // ===== LAYER 6: HTTP/HTTPS/DNS =====

    return pp;
}
