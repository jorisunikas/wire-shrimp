#pragma once

#include "../data_types/headers.hpp"
#include "../data_types/packet.hpp"
#include "../indexer.hpp"

/**
 * Parser for parsing HTTP headers.
 * Extracts HTTP method, URI, and headers.
 */
class HTTPParser {
public:
    /**
     * Parse HTTP header (starts after Ethernet + IPv4 headers)
     * @param rp Raw packet containing the HTTP header
     * @param indexer Indexer for managing parsed headers
     * @return Pointer to parsed HTTPHeader if successful, nullptr otherwise
     */
    HTTPHeader parse(RawPacket& rp, Indexer& indexer);  

    /**
     * Extract the Host URL from the raw HTTP header string
     * @param httpHeader Raw HTTP header string
     * @return Extracted Host URL or empty string if not found
     */
    std::string extractHostURL(HTTPHeader& httpHeader);
    
    /**
     * Validate if the packet contains a valid HTTP header
     * Checks minimum length and HTTP header fields
     * @param rp Raw packet containing the HTTP header
     * @return true if valid HTTP header is present
     */
    bool isValid(RawPacket& rp);
};
