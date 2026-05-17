#pragma once

#include "protocol_parser.hpp"
#include "../data_types/packet.hpp"
#include "../data_types/headers.hpp"
#include <cstddef>

/**
 * Parser for parsing UDP headers.
 * Extracts source/destination ports.
 */
class UDPParser : public ProtocolParser {
public:
    /**
     * Parse UDP header (starts after Ethernet + IPv4 headers)
     * @param rp Raw packet containing the UDP header
     * @return Pointer to parsed UDPHeader
     */
    UDPHeader* parse(RawPacket rp) override;
    
    /**
     * Validate if the packet contains a valid UDP header
     * Checks minimum length and UDP header fields
     * @param rp Raw packet containing the UDP header
     * @return true if valid UDP header is present
     */
    bool isValid(RawPacket rp) override;
};
