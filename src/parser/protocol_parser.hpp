#pragma once

#include <cstdint>
#include <cstddef>

#include "../data_types/headers.hpp"
#include "../data_types/packet.hpp"

static constexpr uint8_t ETHERNET_HEADER_SIZE = 14;
static constexpr uint8_t IPV4_MIN_HEADER_SIZE = 20;
static constexpr uint8_t TCP_MIN_HEADER_SIZE = 20;
static constexpr uint8_t UDP_MIN_HEADER_SIZE = 8;

/**
 * Parser interface for parsing different protocol layers.
 * Allows flexible, composable parsing logic.
 */
class ProtocolParser {
public:
    virtual ~ProtocolParser() = default;
    
    virtual bool isValid(RawPacket rp) = 0;
    virtual Header* parse(RawPacket rp) = 0;
};
