#include "statistics.hpp"
#include <iomanip>
#include <sstream>

void Statistics::add(const ParsedPacket &pp) {
    totalPackets++;
    protocolCounts[pp.protocol]++;
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
    return oss.str();
}
