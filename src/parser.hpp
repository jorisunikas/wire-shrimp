#pragma once

#include "headers.hpp"
#include "packet.hpp"
#include <cstdint>
#include <sys/types.h>

/**
 * Parses RawPacket into ParsedPacket
 */
class Parser {
  public:
    /**
     * Parse a raw packet into a ParsedPacket
     * @param data Pointer to raw bytes starting with ethernet header
     * @param len  Length of raw bytes buffer
     * @return ParsedPacket containing parsed headers
     */
    ParsedPacket parse(const uint8_t *data, int len);

  private:
    EthernetHeader parseEthernet(const uint8_t *data, int offset);
    IPv4Header parseIPv4(const uint8_t *data, int offset);
    TCPHeader parseTCP(const uint8_t *data, int offset);
    UDPHeader parseUDP(const uint8_t *data, int offset);
};
