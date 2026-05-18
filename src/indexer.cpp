#include "indexer.hpp"

void Indexer::addURL(const std::string& url, const std::string& ip) {
    ipMap[ip] = url;
}

std::string Indexer::getURL(const std::string& ip) const {
    if (ipMap.find(ip) != ipMap.end()) {
        return ipMap.at(ip);
    }
    return "";
}