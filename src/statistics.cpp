#include "statistics.hpp"
#include <iomanip>
#include <sstream>

std::string formatTable(const std::string& label, const std::unordered_map<std::string, int>& counts);

void Statistics::add(const ParsedPacket &pp) {
    totalPackets++;
    protocolCounts[pp.protocol]++;

    if(pp.httpData && pp.httpData->hostURL != "") {
        urlCounts[pp.httpData->hostURL]++;
    }
    else if (pp.tlsData && pp.tlsData->sniHeader.serverName != "") {
        urlCounts[pp.tlsData->sniHeader.serverName]++;
    }
    else if (pp.dnsData && pp.dnsData->queryName != "") {
        urlCounts[pp.dnsData->queryName]++;
    }
}

std::string Statistics::getReport() const {
    std::ostringstream oss;
    
    if(totalPackets == 0) {
        return "No packets captured.\n";
    }
    else{
        oss << "\nCaptured: " << totalPackets << " packets\n\n";
        oss << formatTable("Protocol", protocolCounts);
    }
    

    if(urlCounts.empty()) {
        oss << "No URLs captured.\n";
        return oss.str();
    }
    else{
        oss << "\n";
        oss << formatTable("URL", urlCounts);
    }
    
    return oss.str();
}

// === Helper function ===

std::string formatTable(const std::string& label,
                        const std::unordered_map<std::string, int>& counts) {
    // Dynamic column widths
    size_t maxKeyLen = label.size();
    int maxCount     = 0;
    for (const auto& [key, count] : counts) {
        maxKeyLen = std::max(maxKeyLen, key.size());
        maxCount  = std::max(maxCount, count);
    }

    const size_t keyColWidth   = maxKeyLen + 2;
    const size_t countColWidth = std::max(std::to_string(maxCount).size(),
                                          std::string("Count").size()) + 2;
    const size_t totalWidth    = keyColWidth + countColWidth;
    const std::string separator(totalWidth, '-');

    std::ostringstream oss;
    oss << separator << "\n";
    oss << std::left  << std::setw(keyColWidth)   << label
        << std::right << std::setw(countColWidth) << "Count" << "\n";
    oss << separator << "\n";

    for (const auto& [key, count] : counts) {
        oss << std::left  << std::setw(keyColWidth)   << key
            << std::right << std::setw(countColWidth) << count << "\n";
    }

    oss << separator << "\n";
    return oss.str();
}