#pragma once

#include <cstdint>
#include <cstddef>

/**
 * Parser interface for parsing different protocol layers.
 * Allows flexible, composable parsing logic.
 */
class ProtocolParser {
public:
    virtual ~ProtocolParser() = default;
    
    /**
     * Parse protocol data from buffer
     * @param data Pointer to raw bytes
     * @param len Length of buffer
     * @return true if parsing succeeded
     */
    virtual bool parse(const uint8_t *data, size_t len) = 0;
};
