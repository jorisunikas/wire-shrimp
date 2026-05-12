#pragma once

#include "protocol_strategy.hpp"
#include "../headers.hpp"
#include <cstdint>
#include <cstddef>

/**
 * Strategy for parsing UDP headers.
 * Extracts source/destination ports.
 */
class UDPStrategy : public ProtocolStrategy {
private:
    UDPHeader header;

public:
    /**
     * Parse UDP header (starts after Ethernet + IPv4 headers)
     * @param data Pointer to raw bytes at transport layer offset
     * @param len Length of remaining buffer
     * @return true if parsing succeeded
     */
    bool parse(const uint8_t *data, size_t len) override;
    
    /**
     * Get parsed UDP header
     * @return Parsed UDPHeader
     */
    UDPHeader getHeader() const;
};
