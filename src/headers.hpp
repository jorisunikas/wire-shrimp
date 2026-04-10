#pragma once

#include <cstdint>
#include <string>

/*
 * Headers do not reflect real representation of the data. Instead they are
 * constructed during parsing and show only the data which will be displayed.
 */

struct EthernetHeader {
    std::string dstMac;  ///< Destination address (must be first)
    std::string srcMac;  ///< Source address
    u_int16_t etherType; ///< Defines packet type: 0x0800 -> IPv4, 0x0806 ->
                         ///< ARP, 0x86DD -> IPv6
};

struct IPv4Header {
    std::string srcIp; ///< Source IP
    std::string dstIp; ///< Destination IP
    uint8_t protocol;  ///< Defines inner packet protocol: 1 -> ICMP,  6 -> TCP,
                       ///< 17 -> UDP
    uint8_t ttl; ///< Time To Live (Hints at the source OS, where the packet is
                 ///< going)
    uint8_t ihl; ///< Internet Header Length - counts in 32bit increments
                 ///< (usually 5)
};

struct TCPHeader {
    uint16_t srcPort; ///< Source port
    uint16_t dstPort; ///< Destination port
    uint8_t flags;    ///< SYN, ACK, etc.
};

struct UDPHeader {
    uint16_t srcPort; ///< Source port
    uint16_t dstPort; ///< Destination port
};
