#pragma once

#include "../data_types/headers.hpp"
#include "../data_types/packet.hpp"

/**
 * Parser for parsing Ethernet headers.
 * Extracts source/destination MAC addresses and EtherType.
 */
class DNSParser{
public:
    /**
     * Parse DNS header (first 12 bytes of packet)
     * @param rp RawPacket to parse
     * @return Parsed DNSHeader
     */
    DNSHeader parse(RawPacket& rp);
    
    /**
     * Validate if the raw packet has enough bytes for a DNS header
     * @param rp RawPacket to validate
     * @return true if packet is valid, false otherwise
     */
    bool isValid(RawPacket& rp);
};
