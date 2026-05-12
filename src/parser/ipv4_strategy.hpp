#pragma once

#include "protocol_strategy.hpp"
#include "../headers.hpp"
#include <cstdint>
#include <cstddef>
#include <optional>

/**
 * Strategy for parsing IPv4 headers.
 * Extracts source/destination IPs, TTL, protocol, and IHL.
 */
class IPv4Strategy : public ProtocolStrategy {
private:
    IPv4Header header;

public:
    /**
     * Parse IPv4 header (starts at offset 14 bytes from packet start)
     * @param data Pointer to raw bytes at offset 14
     * @param len Length of remaining buffer
     * @return true if parsing succeeded
     */
    bool parse(const uint8_t *data, size_t len) override;
    
    /**
     * Get parsed IPv4 header
     * @return Parsed IPv4Header
     */
    IPv4Header getHeader() const;
    
    /**
     * Get size of this IPv4 header in bytes
     * @return Header size (typically 20 bytes)
     */
    uint8_t getHeaderSize() const;
};
