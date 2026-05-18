#pragma once

#include "data_types/packet.hpp"

/**
 * Parses RawPacket into ParsedPacket using Parser pattern.
 *
 * The Parser delegates parsing of each protocol layer to specialized
 * parser objects (EthernetParser, IPv4Parser, etc.), allowing
 * flexible composition and easy extension to new protocols.
 */
class Parser {
  public:
    /**
     * Parse a raw packet into a ParsedPacket
     * @param rp RawPacket to parse
     * @return ParsedPacket containing parsed headers
     */
    static ParsedPacket parse(RawPacket rp);
};
