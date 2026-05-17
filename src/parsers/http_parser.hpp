#pragma once

#include "../data_types/headers.hpp"
#include "../data_types/packet.hpp"

/**
 * Parser for parsing HTTP headers.
 * Extracts HTTP method, URI, and headers.
 */
class HTTPParser {
public:
    /**
     * Parse HTTP header (starts after Ethernet + IPv4 headers)
     * @param rp Raw packet containing the HTTP header
     * @return Pointer to parsed HTTPHeader if successful, nullptr otherwise
     */
    HTTPHeader parse(RawPacket rp);
    
    /**
     * Validate if the packet contains a valid HTTP header
     * Checks minimum length and HTTP header fields
     * @param rp Raw packet containing the HTTP header
     * @return true if valid HTTP header is present
     */
    bool isValid(RawPacket rp);
};
