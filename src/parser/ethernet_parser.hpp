#pragma once

#include "protocol_parser.hpp"
#include "../headers.hpp"
#include <cstdint>
#include <cstddef>

/**
 * Parser for parsing Ethernet headers.
 * Extracts source/destination MAC addresses and EtherType.
 */
class EthernetParser : public ProtocolParser {
private:
    EthernetHeader header;

public:
    /**
     * Parse Ethernet header (first 14 bytes of packet)
     * @param data Pointer to raw bytes at offset 0
     * @param len Length of buffer
     * @return true if parsing succeeded
     */
    bool parse(const uint8_t *data, size_t len) override;
    
    /**
     * Get parsed Ethernet header
     * @return Parsed EthernetHeader
     */
    EthernetHeader getHeader() const;
};
