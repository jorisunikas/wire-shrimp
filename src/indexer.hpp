#pragma once

/**
 * Manages the url and ip addresser as memory
 */

#include <string>
#include <unordered_map>

class Indexer {
  protected:
    static std::unordered_map<std::string, std::string> ipMap;

  public:
    void addURL(std::string ip, std::string url);
    std::string getURL(std::string ip);
};
