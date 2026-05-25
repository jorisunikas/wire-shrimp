#pragma once

#include "data_types/packet.hpp"
#include <string>
#include <unordered_map>

class Statistics {
public:
    void add(const ParsedPacket &pp);
    std::string getReport() const;

private:
    int totalPackets = 0;
    std::unordered_map<std::string, int> protocolCounts;
};
