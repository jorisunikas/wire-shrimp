#pragma once

#include "data_types/packet.hpp"
#include "data_types/headers.hpp"
#include "parsers/ethernet_parser.hpp"
#include "parsers/ipv4_parser.hpp"
#include "parsers/tcp_parser.hpp"
#include "parsers/udp_parser.hpp"
#include "parsers/http_parser.hpp"

#include <memory>

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

private:
    // Parser instances (created as needed)
    static std::unique_ptr<EthernetParser> ethernetParser;
    static std::unique_ptr<IPv4Parser> ipv4Parser;
    static std::unique_ptr<TCPParser> tcpParser;
    static std::unique_ptr<UDPParser> udpParser;
    static std::unique_ptr<HTTPParser> httpParser;

    /**
     * Initialize parsers on first use
     */
    static void initializeParsers();
};
