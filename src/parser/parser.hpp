#pragma once

#include "../packet.hpp"
#include "ethernet_strategy.hpp"
#include "ipv4_strategy.hpp"
#include "tcp_strategy.hpp"
#include "udp_strategy.hpp"
#include <cstdint>
#include <sys/types.h>
#include <memory>

/**
 * Parses RawPacket into ParsedPacket using Strategy pattern.
 * 
 * The Parser delegates parsing of each protocol layer to specialized
 * strategy objects (EthernetStrategy, IPv4Strategy, etc.), allowing
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
    // Strategy instances (created as needed)
    static std::unique_ptr<EthernetStrategy> ethernetStrategy;
    static std::unique_ptr<IPv4Strategy> ipv4Strategy;
    static std::unique_ptr<TCPStrategy> tcpStrategy;
    static std::unique_ptr<UDPStrategy> udpStrategy;

    /**
     * Initialize strategies on first use
     */
    static void initializeStrategies();
};
