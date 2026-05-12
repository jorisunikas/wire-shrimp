#pragma once

#include "protocol_strategy.hpp"
#include "../headers.hpp"
#include <cstdint>
#include <cstddef>

/**
 * Strategy for parsing TCP headers.
 * Extracts source/destination ports and TCP flags.
 */
class TCPStrategy : public ProtocolStrategy {
private:
    TCPHeader header;

public:
    /**
     * Parse TCP header (starts after Ethernet + IPv4 headers)
     * @param data Pointer to raw bytes at transport layer offset
     * @param len Length of remaining buffer
     * @return true if parsing succeeded
     */
    bool parse(const uint8_t *data, size_t len) override;
    
    /**
     * Get parsed TCP header
     * @return Parsed TCPHeader
     */
    TCPHeader getHeader() const;
};
