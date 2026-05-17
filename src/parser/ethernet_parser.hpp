#pragma once

#include "protocol_parser.hpp"
#include "../data_types/headers.hpp"
<<<<<<< HEAD
#include "../data_types/packet.hpp"
#include <cstddef>
=======
>>>>>>> 0b0316f (Import cleaning)

/**
 * Parser for parsing Ethernet headers.
 * Extracts source/destination MAC addresses and EtherType.
 */
class EthernetParser : public ProtocolParser {
public:
    /**
     * Parse Ethernet header (first 14 bytes of packet)
     * @param rp RawPacket to parse
     * @return Parsed EthernetHeader
     */
    EthernetHeader* parse(RawPacket rp) override;
    
    /**
     * Validate if the raw packet has enough bytes for an Ethernet header
     * @param rp RawPacket to validate
     * @return true if packet is valid, false otherwise
     */
    bool isValid(RawPacket rp) override;
};
