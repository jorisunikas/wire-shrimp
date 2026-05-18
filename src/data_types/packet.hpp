#pragma once

#include "headers.hpp"
#include <optional>
#include <string>

struct RawPacket {
    const uint8_t *data; ///< Pointer to raw bytes of the packet
    size_t len;          ///< Length of the raw byte buffer
};

struct ParsedPacket {
    EthernetHeader ethData;             ///< Parsed Ethernet header
    std::optional<IPv4Header> IPv4Data; ///< Present only if IPv4
    std::optional<IPv6Header> IPv6Data; ///< Present only if IPv6
    std::optional<TCPHeader> tcpData;   ///< Present only if TCP
    std::optional<UDPHeader> udpData;   ///< Present only if UDP
    std::optional<HTTPHeader> httpData; ///< Present only if HTTP

    std::string protocol; ///< Human readable protocol: "TCP", "UDP", etc.
    bool valid = false;   ///< True if packet has been parsed correctly
};
