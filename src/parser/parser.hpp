#pragma once

#include "../packet.hpp"
#include "ethernet_parser.hpp"
#include "ipv4_parser.hpp"
#include "tcp_parser.hpp"
#include "udp_parser.hpp"
#include <cstdint>
#include <sys/types.h>
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
     * @param data Pointer to raw bytes starting with ethernet header
     * @param len  Length of raw bytes buffer
     * @return ParsedPacket containing parsed headers
     */
    static ParsedPacket parse(const uint8_t *data, size_t len);

private:
    // Parser instances (created as needed)
    static std::unique_ptr<EthernetParser> ethernetParser;
    static std::unique_ptr<IPv4Parser> ipv4Parser;
    static std::unique_ptr<TCPParser> tcpParser;
    static std::unique_ptr<UDPParser> udpParser;

    /**
     * Initialize parsers on first use
     */
    static void initializeParsers();
};
