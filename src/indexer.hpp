#pragma once

/**
 * Manages the url and ip addresser as memory
 */

#include <unordered_map>
#include <string>

class Indexer {
private:
    std::unordered_map<std::string, std::string> ipMap;;

public:
    void addURL(const std::string& url, const std::string& ip);
    std::string getURL(const std::string& ip) const;
};