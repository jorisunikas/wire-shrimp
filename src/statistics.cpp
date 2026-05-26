#include "statistics.hpp"
#include <iomanip>
#include <sstream>

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
    oss << "\nCaptured: " << totalPackets << " packets\n";
    oss << std::left << std::setw(20) << "Protocol"
        << std::right << std::setw(10) << "Count" << "\n";
    oss << std::string(30, '-') << "\n";
    for (const auto &[proto, count] : protocolCounts) {
        oss << std::left << std::setw(20) << proto
            << std::right << std::setw(10) << count << "\n";
    }
    oss << std::string(30, '-') << "\n";
    oss << std::left << std::setw(20) << "URL"
        << std::right << std::setw(10) << "Count" << "\n";
    oss << std::string(30, '-') << "\n";
    for (const auto &[url, count] : urlCounts) {
        oss << std::left << std::setw(20) << url
            << std::right << std::setw(10) << count << "\n";
    }
    return oss.str();
}
