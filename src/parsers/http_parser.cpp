#include "http_parser.hpp"

#include <regex>

HTTPHeader HTTPParser::parse(RawPacket& rp, Indexer& indexer) {
    const uint8_t* httpData = rp.data + ETHERNET_HEADER_SIZE + IPV4_MIN_HEADER_SIZE + TCP_MIN_HEADER_SIZE;
    size_t httpDataLen = rp.len - ETHERNET_HEADER_SIZE - IPV4_MIN_HEADER_SIZE - TCP_MIN_HEADER_SIZE;

    HTTPHeader header;
    header.details = std::string(reinterpret_cast<const char*>(httpData), httpDataLen);

    header.hostURL = extractHostURL(header);

    return header;
}

std::string HTTPParser::extractHostURL(HTTPHeader& httpHeader) {
    std::regex hostRegex("Host: ([^\r\n]+)");
    std::smatch match;
    if (std::regex_search(httpHeader.details, match, hostRegex)) {
        return match[1].str();
    }
    return "";
}

bool HTTPParser::isValid(RawPacket& rp) {
    const uint8_t* httpData = rp.data + ETHERNET_HEADER_SIZE + IPV4_MIN_HEADER_SIZE + TCP_MIN_HEADER_SIZE;
    size_t httpDataLen = rp.len - ETHERNET_HEADER_SIZE - IPV4_MIN_HEADER_SIZE - TCP_MIN_HEADER_SIZE;

    if (httpDataLen < 14) return false; // Minimum length for HTTP method + URL + version

    std::string httpStr(reinterpret_cast<const char*>(httpData), httpDataLen);
    size_t methodEnd = httpStr.find(' ');
    size_t urlEnd = httpStr.find(' ', methodEnd + 1);
    size_t versionEnd = httpStr.find("\r\n", urlEnd + 1);

    return methodEnd != std::string::npos && urlEnd != std::string::npos && versionEnd != std::string::npos;
}