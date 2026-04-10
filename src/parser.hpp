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
    ParsedPacket parse(const uint8_t *data, size_t len);

  private:
    /**
     * Parses first 14 bytes into EthernetHeader
     * @param data Byte buffer with offset 0
     */
    EthernetHeader parseEthernet(const uint8_t *data);
    /**
     * Parse IPv4 header
     * @param data Byte buffer offset by 14
     */
    IPv4Header parseIPv4(const uint8_t *data);
    /*
     * Parse TCP header
     * @param data Byte buffer offset by (14 + 4 * ihl)
     */
    TCPHeader parseTCP(const uint8_t *data);
    /*
     * Parse UDP header
     * @param data Byte buffer offset by (14 + 4 * ihl)
     */
    UDPHeader parseUDP(const uint8_t *data);
};
