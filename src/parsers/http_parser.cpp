#include "http_parser.hpp"

HTTPHeader HTTPParser::parse(RawPacket rp) {
    const uint8_t* httpData = rp.data + ETHERNET_HEADER_SIZE + IPV4_MIN_HEADER_SIZE + TCP_MIN_HEADER_SIZE;
    size_t httpDataLen = rp.len - ETHERNET_HEADER_SIZE - IPV4_MIN_HEADER_SIZE - TCP_MIN_HEADER_SIZE;

    HTTPHeader header;
    header.details = std::string(reinterpret_cast<const char*>(httpData), httpDataLen);

    // Finds "http://" from string and extracts the URL part
    size_t hostPos = header.details.find("http://");
    if (hostPos != std::string::npos) {
        hostPos += 7; // Move past "http://"
        size_t hostEnd = header.details.find("/", hostPos);
        if (hostEnd != std::string::npos) {
            header.hostURL = header.details.substr(hostPos-7, hostEnd - hostPos);
        }
    }

    return header;
}

bool HTTPParser::isValid(RawPacket rp) {
    const uint8_t* httpData = rp.data + ETHERNET_HEADER_SIZE + IPV4_MIN_HEADER_SIZE + TCP_MIN_HEADER_SIZE;
    size_t httpDataLen = rp.len - ETHERNET_HEADER_SIZE - IPV4_MIN_HEADER_SIZE - TCP_MIN_HEADER_SIZE;

    if (httpDataLen < 14) return false; // Minimum length for HTTP method + URL + version

    std::string httpStr(reinterpret_cast<const char*>(httpData), httpDataLen);
    size_t methodEnd = httpStr.find(' ');
    size_t urlEnd = httpStr.find(' ', methodEnd + 1);
    size_t versionEnd = httpStr.find("\r\n", urlEnd + 1);

    return methodEnd != std::string::npos && urlEnd != std::string::npos && versionEnd != std::string::npos;
}