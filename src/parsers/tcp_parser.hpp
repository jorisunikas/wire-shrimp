#pragma once

#include "../data_types/headers.hpp"
#include "../data_types/packet.hpp"
#include <cstddef>

/**
 * Parser for parsing TCP headers.
 * Extracts source/destination ports and TCP flags.
 */
class TCPParser {
public:
    /**
     * Parse TCP header (starts after Ethernet + IPv4 headers)
     * @param rp Raw packet containing the TCP header
     * @return Pointer to parsed TCPHeader if successful, nullptr otherwise
     */
    TCPHeader parse(RawPacket rp);
    
    /**
     * Validate if the packet contains a valid TCP header
     * Checks minimum length and TCP header fields
     * @param rp Raw packet containing the TCP header
     * @return true if valid TCP header is present
     */
    bool isValid(RawPacket rp);
};
