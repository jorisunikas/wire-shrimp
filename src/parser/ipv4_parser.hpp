#pragma once

#include "protocol_parser.hpp"
#include "../data_types/headers.hpp"
#include "../data_types/packet.hpp"
#include <cstdint>
#include <cstddef>
#include <optional>

/**
 * Parser for parsing IPv4 headers.
 * Extracts source/destination IPs, TTL, protocol, and IHL.
 */
class IPv4Parser : public ProtocolParser {
public:
    /**
     * Parse IPv4 header (starts at offset 14 bytes from packet start)
     * @param rp Raw packet containing the IPv4 header
     * @return Pointer to parsed IPv4Header
     */
    IPv4Header* parse(RawPacket rp) override;

    /**
     * Validate if the packet contains a valid IPv4 header
     * Checks minimum length and version field
     * @param rp Raw packet containing the IPv4 header
     * @return true if valid IPv4 header is present
     */
    bool isValid(RawPacket rp) override;    
};
