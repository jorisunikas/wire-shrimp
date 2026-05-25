#include "dns_parser.hpp"

DNSHeader DNSParser::parse(RawPacket& rp) {
    DNSHeader header;
    const uint8_t *data = rp.data + ETHERNET_HEADER_SIZE + IPV4_MIN_HEADER_SIZE + UDP_MIN_HEADER_SIZE; // DNS header starts after Ethernet + IPv4 + UDP headers
    size_t len = rp.len;

    header.transactionID = (data[0] << 8) | data[1];
    header.flags = (data[2] << 8) | data[3];
    header.questionCount = (data[4] << 8) | data[5];
    header.answerCount = (data[6] << 8) | data[7];
    header.authorityCount = (data[8] << 8) | data[9];
    header.additionalCount = (data[10] << 8) | data[11];

    // For simplicity, we will only parse the first question (if it exists)
    if (header.questionCount > 0) {
        size_t offset = 12; // Start of question section
        std::string queryName;
        header.queryName = ""; // or however your DNSHeader initialises it
        while (offset < len && data[offset] != 0) {
            uint8_t labelLength = data[offset++];
            if (offset + labelLength > len) break;
            if (!header.queryName.empty()) header.queryName.append(".");
            header.queryName.append((char*)(data + offset), labelLength);
            offset += labelLength;
        }
    }

    return header;
}

bool DNSParser::isValid(RawPacket& rp) {
    if (rp.len < DNS_MIN_HEADER_SIZE) {
        return false; // Not enough data for minimum DNS header
    }
    // Additional checks can be added here (e.g. valid ports)
    return true;
}
