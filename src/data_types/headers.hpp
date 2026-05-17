#pragma once

#include <cstdint>
#include <string>

static constexpr uint8_t ETHERNET_HEADER_SIZE = 14;
static constexpr uint8_t IPV4_MIN_HEADER_SIZE = 20;
static constexpr uint8_t TCP_MIN_HEADER_SIZE = 20;
static constexpr uint8_t UDP_MIN_HEADER_SIZE = 8;
static constexpr uint8_t IPV6_MIN_HEADER_SIZE = 40;

/*
 * Headers do not reflect real representation of the data. Instead they are
 * constructed during parsing and show only the data which will be displayed.
 */
struct Header{  
    virtual ~Header() = default;

};

struct EthernetHeader : Header {
    std::string dstMac;  ///< Destination address (must be first)
    std::string srcMac;  ///< Source address
    std::string etherTypeStr; ///< Human readable EtherType (e.g. "IPv4", "ARP")
    uint16_t etherType; ///< Defines packet type: 0x0800 -> IPv4, 0x0806 ->
                         ///< ARP, 0x86DD -> IPv6
};

struct IPv6Header : Header {
    std::string srcIp;
    std::string dstIp;
    uint8_t protocol;       ///< defines inner packet protocol
    uint8_t hopLimit;       ///< same as TTL for IPv4
    uint16_t payloadLength; ///< defines payload length
};

struct IPv4Header : Header {
    std::string srcIp; ///< Source IP
    std::string dstIp; ///< Destination IP
    uint8_t protocol;  ///< Defines inner packet protocol: 1 -> ICMP,  6 -> TCP,
                       ///< 17 -> UDP
    uint8_t ttl; ///< Time To Live (Hints at the source OS, where the packet is
                 ///< going)
    uint8_t ihl; ///< Internet Header Length - counts in 32bit increments
                 ///< (usually 5)
};

struct TCPHeader : Header {
    uint16_t srcPort; ///< Source port
    uint16_t dstPort; ///< Destination port
    uint8_t flags;    ///< SYN, ACK, etc.
};

struct UDPHeader : Header {
    uint16_t srcPort; ///< Source port
    uint16_t dstPort; ///< Destination port
    uint16_t length;  ///< UDP packet length
    uint16_t checksum;///< UDP checksum
};

struct HTTPHeader : Header {
    std::string details; ///< Raw HTTP header details (for display)
    std::string hostURL;    ///< HTTP Host header value (for filtering)
};