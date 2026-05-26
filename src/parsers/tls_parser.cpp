#include "tls_parser.hpp"
#include <iostream>

namespace {

struct PacketReader {
    const uint8_t *data;
    size_t len;
    size_t offset = 0;

    bool has(size_t n) const { return offset + n <= len; }
    uint8_t u8() { return data[offset++]; }
    uint16_t u16() {
        uint16_t val = (data[offset] << 8) | data[offset + 1];
        offset += 2;
        return val;
    }
    void skip(size_t n) { offset += n; }
    std::string str(size_t n) {
        std::string s(reinterpret_cast<const char *>(data + offset), n);
        offset += n;
        return s;
    }
};

std::string versionToString(uint16_t version) {
    switch (version) {
    case 0x0300: return "SSL 3.0";
    case 0x0301: return "TLS 1.0";
    case 0x0302: return "TLS 1.1";
    case 0x0303: return "TLS 1.2";
    case 0x0304: return "TLS 1.3";
    default:     return "N/A";
    }
}

void parseClientHelloSNI(PacketReader &r, TLSHeader &header) {
    // Version (2) + Random (32)
    if (!r.has(34)) return;
    r.skip(34);

    // Session ID (1-byte length prefix)
    if (!r.has(1)) return;
    r.skip(r.u8());

    // Cipher Suites (2-byte length prefix)
    if (!r.has(2)) return;
    r.skip(r.u16());

    // Compression Methods (1-byte length prefix)
    if (!r.has(1)) return;
    r.skip(r.u8());

    // Extensions total length
    if (!r.has(2)) return;
    uint16_t extTotalLen = r.u16();
    size_t extEnd = r.offset + extTotalLen;

    // Walk extensions looking for SNI (type 0x0000)
    while (r.offset + 4 <= extEnd && r.has(4)) {
        uint16_t extType = r.u16();
        uint16_t extLen = r.u16();

        if (extType != 0x0000) {
            r.skip(extLen);
            continue;
        }

        // SNI extension found
        if (!r.has(5)) break;
        header.sniHeader.listLength = r.u16();
        header.sniHeader.nameType = r.u8();
        header.sniHeader.nameLength = r.u16();

        if (header.sniHeader.nameType == 0x00 &&
            r.has(header.sniHeader.nameLength)) {
            header.sniHeader.serverName = r.str(header.sniHeader.nameLength);
        }
        break;
    }
}

} // namespace

TLSHeader TLSParser::parse(RawPacket &rp) {
    TLSHeader header;

    // Calculate dynamic header lengths to find TLS payload
    const uint8_t *ip_packet = rp.data + ETHERNET_HEADER_SIZE;
    size_t ip_header_len = (ip_packet[0] & 0x0F) * 4;

    const uint8_t *tcp_packet = ip_packet + ip_header_len;
    size_t tcp_header_len = ((tcp_packet[12] >> 4) & 0x0F) * 4;

    const uint8_t *data = tcp_packet + tcp_header_len;
    size_t len = rp.len - ETHERNET_HEADER_SIZE - ip_header_len - tcp_header_len;

    // Parse TLS Record header (5 bytes)
    header.recordHeader.contentType = data[0];
    header.recordHeader.version = (data[1] << 8) | data[2];
    header.recordHeader.versionStr = versionToString(header.recordHeader.version);
    header.recordHeader.length = (data[3] << 8) | data[4];

    // Only proceed if this is a Handshake record with enough data
    if (header.recordHeader.contentType != 0x16 || len < 9) {
        header.isHandshake = false;
        return header;
    }

    header.isHandshake = true;
    header.handshakeHeader.handshakeType = data[5];
    header.handshakeHeader.length[0] = data[6];
    header.handshakeHeader.length[1] = data[7];
    header.handshakeHeader.length[2] = data[8];

    // SNI is only in Client Hello (type 0x01)
    if (header.handshakeHeader.handshakeType == 0x01) {
        PacketReader r{data, len, 5 + 4}; // after Record + Handshake headers
        parseClientHelloSNI(r, header);
    }

    return header;
}

bool TLSParser::isValid(RawPacket &rp) {
    if (rp.len <
        ETHERNET_HEADER_SIZE + IPV4_MIN_HEADER_SIZE + TCP_MIN_HEADER_SIZE) {
        return false;
    }

    const uint8_t *ip_packet = rp.data + ETHERNET_HEADER_SIZE;
    size_t ip_header_len = (ip_packet[0] & 0x0F) * 4;

    if (ETHERNET_HEADER_SIZE + ip_header_len + TCP_MIN_HEADER_SIZE > rp.len) {
        return false;
    }

    const uint8_t *tcp_packet = ip_packet + ip_header_len;
    size_t tcp_header_len = ((tcp_packet[12] >> 4) & 0x0F) * 4;

    if (rp.len < ETHERNET_HEADER_SIZE + ip_header_len + tcp_header_len +
                     TLS_MIN_HEADER_SIZE) {
        return false;
    }

    return true;
}
