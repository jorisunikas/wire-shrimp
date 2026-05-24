#pragma once

#include "../data_types/headers.hpp"
#include "../data_types/packet.hpp"
#include <cstddef>

/**
 * Parser for parsing TLS headers.
 * Extracts source/destination ports and TLS flags.
 */
class TLSParser {
public:
    /**
     * Parse TLS header (starts after Ethernet + IPv4 headers)
     * @param rp Raw packet containing the TLS header
     * @return Pointer to parsed TLSHeader if successful, nullptr otherwise
     */
    TLSHeader parse(RawPacket& rp);
    
    /**
     * Validate if the packet contains a valid TLS header
     * Checks minimum length and TLS header fields
     * @param rp Raw packet containing the TLS header
     * @return true if valid TLS header is present
     */
    bool isValid(RawPacket& rp);
};