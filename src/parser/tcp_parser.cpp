#include "tcp_parser.hpp"

#include <algorithm>
#include <iostream>
#include <vector>


static constexpr uint8_t TCP_MIN_HEADER_SIZE = 20;

std::string extractHostname(const uint8_t* data, size_t len);

bool TCPParser::parse(const uint8_t *data, size_t len) {
    if (len < TCP_MIN_HEADER_SIZE) {
        return false;
    }

    header.srcPort = (data[0] << 8) | data[1];
    header.dstPort = (data[2] << 8) | data[3];
    header.flags = data[13];
    header.hostname = extractHostname(data, len);
    
    return true;
}

TCPHeader TCPParser::getHeader() const {
    return header;
}

std::string extractHostname(const uint8_t* data, size_t len) {
    if (len < 40) return "";
    
    // Get IPv4 header length
    uint8_t ipv4_len = (data[0] & 0x0F) * 4;
    if (ipv4_len > len) return "";
    
    // Get TCP header length
    uint8_t tcp_len = ((data[ipv4_len + 12] >> 4) & 0x0F) * 4;
    if (ipv4_len + tcp_len > len) return "";
    
    // HTTP payload starts after TCP header
    size_t http_offset = ipv4_len + tcp_len;
    if (http_offset >= len) return "";
    
    // Convert to string for searching
    std::string payload((const char*)(data + http_offset), len - http_offset);
    
    // Find "Host: " header
    size_t pos = payload.find("Host: ");
    if (pos == std::string::npos) return "";
    
    pos += 6;  // Skip "Host: "
    
    // Find end of line
    size_t end = payload.find("\r\n", pos);
    if (end == std::string::npos) {
        end = payload.find("\n", pos);
    }
    if (end == std::string::npos) {
        end = payload.length();
    }
    
    // Extract and trim
    std::string hostname = payload.substr(pos, end - pos);
    while (!hostname.empty() && (hostname.back() == '\r' || hostname.back() == ' ')) {
        hostname.pop_back();
    }
    
    return hostname;
}
