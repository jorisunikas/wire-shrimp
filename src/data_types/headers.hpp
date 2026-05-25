#pragma once

#include <cstdint>
#include <string>

static constexpr uint8_t ETHERNET_HEADER_SIZE = 14;
static constexpr uint8_t IPV4_MIN_HEADER_SIZE = 20;
static constexpr uint8_t TCP_MIN_HEADER_SIZE = 20;
static constexpr uint8_t UDP_MIN_HEADER_SIZE = 8;
static constexpr uint8_t IPV6_MIN_HEADER_SIZE = 40;
static constexpr uint8_t TLS_MIN_HEADER_SIZE = 5;
static constexpr uint8_t HTTP_MIN_HEADER_SIZE = 4; 
static constexpr uint8_t DNS_MIN_HEADER_SIZE = 12;

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

struct TLSRecordHeader {
    uint8_t  contentType;  // 0x16 for Handshake
    uint16_t version;      // TLS version (e.g., 0x0303 for TLS 1.2)
    std::string versionStr; // Human readable version (e.g., "TLS 1.2")
    uint16_t length;       // Length of data following this header
};
struct TLSHandshakeHeader {
    uint8_t  handshakeType; // 0x01 for Client Hello
    uint8_t  length[3];     // 24-bit integer tracking message length
    
    // Helper method because C++ has no native uint24_t type
    uint32_t getLength() const {
        return (length[0] << 16) | (length[1] << 8) | length[2];
    }
};
struct TLSExtensionHeader {
    uint16_t type;         // Extension type (0x0000 for SNI)
    uint16_t length;       // Length of this specific extension's data
};
struct TLSSniHeader {
    uint16_t listLength;   // Length of the server name list
    uint8_t  nameType;     // 0x00 for hostname
    uint16_t nameLength;   // Length of the plain-text string
    std::string serverName; // The actual server name (e.g., "example.com")
};

struct TLSHeader : Header {
    TLSRecordHeader recordHeader;
    TLSHandshakeHeader handshakeHeader;
    TLSExtensionHeader extensionHeader;
    TLSSniHeader sniHeader;
    std::string hostURL; // Extracted from SNI or HTTP Host header for easier filtering
    bool isHandshake = false;
};

struct DNSHeader : Header {
    uint16_t transactionID;
    uint16_t flags;
    uint16_t questionCount;
    uint16_t answerCount;
    uint16_t authorityCount;
    uint16_t additionalCount;

    std::string queryName; 
    uint16_t queryType;
};