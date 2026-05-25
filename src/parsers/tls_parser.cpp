#include "tls_parser.hpp"
#include <iostream>

TLSHeader TLSParser::parse(RawPacket& rp) {
    TLSHeader header;
    
    // Dynamically calculate IPv4 Header Length (IHL)
    const uint8_t *ip_packet = rp.data + ETHERNET_HEADER_SIZE;
    size_t ip_header_len = (ip_packet[0] & 0x0F) * 4;

    // Dynamically calculate TCP Header Length (Data Offset)
    const uint8_t *tcp_packet = ip_packet + ip_header_len;
    size_t tcp_header_len = ((tcp_packet[12] >> 4) & 0x0F) * 4;

    // TLS header starts after the dynamically sized IP and TCP headers
    const uint8_t *data = tcp_packet + tcp_header_len;
    size_t len = rp.len - ETHERNET_HEADER_SIZE - ip_header_len - tcp_header_len;

    header.recordHeader.contentType = data[0];
    header.recordHeader.version = (data[1] << 8) | data[2];
    
    switch (header.recordHeader.version) {
        case 0x0300: header.recordHeader.versionStr = "SSL 3.0"; break;
        case 0x0301: header.recordHeader.versionStr = "TLS 1.0"; break;
        case 0x0302: header.recordHeader.versionStr = "TLS 1.1"; break;
        case 0x0303: header.recordHeader.versionStr = "TLS 1.2"; break;
        case 0x0304: header.recordHeader.versionStr = "TLS 1.3"; break;    
        default:     header.recordHeader.versionStr = "N/A"; break;
    }
    
    header.recordHeader.length = (data[3] << 8) | data[4];

    // Check if it's a handshake and ensure we have at least 9 bytes (5 bytes Record + 4 bytes Handshake)
    if(header.recordHeader.contentType == 0x16 && len >= 9) { 
        header.isHandshake = true;
        header.handshakeHeader.handshakeType = data[5];
        header.handshakeHeader.length[0] = data[6];
        header.handshakeHeader.length[1] = data[7];
        header.handshakeHeader.length[2] = data[8];

        // SNI is only present in a Client Hello (Handshake Type 1)
        if (header.handshakeHeader.handshakeType == 0x01) {
            size_t offset = 5 + 4; // Start right after the Handshake header
            
            // Skip Version (2 bytes) + Random (32 bytes) = 34 bytes
            if (offset + 34 <= len) {
                offset += 34;

                // Skip Session ID
                if (offset < len) {
                    uint8_t sid_len = data[offset];
                    offset += 1 + sid_len;

                    // Skip Cipher Suites
                    if (offset + 2 <= len) {
                        uint16_t cs_len = (data[offset] << 8) | data[offset + 1];
                        offset += 2 + cs_len;

                        // Skip Compression Methods
                        if (offset < len) {
                            uint8_t cm_len = data[offset];
                            offset += 1 + cm_len;

                            // We have reached the Extensions!
                            if (offset + 2 <= len) {
                                uint16_t ext_total_len = (data[offset] << 8) | data[offset + 1];
                                size_t ext_end = offset + 2 + ext_total_len;
                                offset += 2;

                                // Iterate through all extensions until we find SNI
                                while (offset + 4 <= ext_end && offset + 4 <= len) {
                                    uint16_t ext_type = (data[offset] << 8) | data[offset + 1];
                                    uint16_t ext_length = (data[offset + 2] << 8) | data[offset + 3];
                                    offset += 4;

                                    if (ext_type == 0x0000) { // Found SNI
                                        size_t sniOffset = offset;
                                        if (sniOffset + 5 <= len && sniOffset + 5 <= ext_end) {
                                            header.sniHeader.listLength = (data[sniOffset] << 8) | data[sniOffset + 1];
                                            header.sniHeader.nameType = data[sniOffset + 2];
                                            header.sniHeader.nameLength = (data[sniOffset + 3] << 8) | data[sniOffset + 4];
                                            
                                            // Extract server name if nameType is 0 (host_name) and it fits
                                            if (header.sniHeader.nameType == 0x00 && sniOffset + 5 + header.sniHeader.nameLength <= len) {
                                                header.sniHeader.serverName = std::string(reinterpret_cast<const char*>(data + sniOffset + 5), header.sniHeader.nameLength);                                            }
                                        }
                                        break; // We found the SNI, no need to keep checking other extensions
                                    }
                                    // Move to the next extension
                                    offset += ext_length;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else {
        header.isHandshake = false;
    }

    return header;
}

bool TLSParser::isValid(RawPacket& rp) {
    if(rp.len < ETHERNET_HEADER_SIZE + IPV4_MIN_HEADER_SIZE + TCP_MIN_HEADER_SIZE) {
        return false; // Not enough data for basic L2/L3/L4 processing
    }

    // Extract dynamic packet lengths to safely validate the TLS minimum size boundaries
    const uint8_t *ip_packet = rp.data + ETHERNET_HEADER_SIZE;
    size_t ip_header_len = (ip_packet[0] & 0x0F) * 4;

    // Bounds check to ensure the IP packet claims a safe length before accessing TCP
    if (ETHERNET_HEADER_SIZE + ip_header_len + TCP_MIN_HEADER_SIZE > rp.len) {
        return false;
    }

    const uint8_t *tcp_packet = ip_packet + ip_header_len;
    size_t tcp_header_len = ((tcp_packet[12] >> 4) & 0x0F) * 4;

    if(rp.len < ETHERNET_HEADER_SIZE + ip_header_len + tcp_header_len + TLS_MIN_HEADER_SIZE) {
        return false; // Not enough data for minimum TLS header
    }
    return true;
}