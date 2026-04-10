#pragma once

#include "headers.hpp"
#include <optional>
#include <string>

struct ParsedPacket {
    EthernetHeader eth;           ///< Parsed Ethernet header
    std::optional<IPv4Header> ip; ///< Present only if IPv4
    std::optional<TCPHeader> tcp; ///< Present only if TCP
    std::optional<UDPHeader> udp; ///< Present only if UDP
    std::string protocol; ///< Human readable protocol: "TCP", "UDP", etc.
    bool valid = false;   ///< True if packet has been parsed correctly
};
