#pragma once

#include "../data_types/headers.hpp"
#include "../data_types/packet.hpp"
#include <cstddef>

/**
 * Parser for parsing IPv6 headers.
 * Extracts source/destination IPs, Hop limit, protocol, and packet length.
 */
class IPv6Parser {
public:
    /**
     * Parse IPv6 header (starts at offset 14 bytes from packet start)
     * @param rp Raw packet containing the IPv6 header
     * @return Parsed IPv6Header
     */
    IPv6Header parse(RawPacket& rp);

    /**
     * Checks minimum length 
     * @param rp Raw packet containing the IPv6 header
     * @return true if valid IPv6 header is present
     */
    bool isValid(RawPacket& rp);    
};
