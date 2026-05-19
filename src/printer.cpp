#include "printer.hpp"
#include <iostream>
#include <iomanip>

void Printer::printPacket(ParsedPacket packet) {
    // Only print HTTP packets for now
    if(packet.protocol.find("HTTP") == std::string::npos) {
        return;
    }



    if (!packet.valid) {
        std::cout << "[-] Invalid or malformed packet captured.\n";
        std::cout << "--------------------------------------------------\n";
        return;
    }

    std::cout << "[+] Packet Captured | Protocol: " << packet.protocol << "\n";

    std::cout << "    [ETH]  " << packet.ethData.srcMac << " -> " << packet.ethData.dstMac 
         << " (Type: 0x" << std::hex << std::setfill('0') << std::setw(4) << packet.ethData.etherType << std::dec << ")\n";

    if (packet.IPv4Data.has_value()) {
        std::cout << "    [IPv4] " << packet.IPv4Data->srcIp << " -> " << packet.IPv4Data->dstIp 
             << " (TTL: " << (int)packet.IPv4Data->ttl << ")\n";
    }

    if (packet.tcpData.has_value()) {
        std::cout << "    [TCP]  Port " << packet.tcpData->srcPort << " -> " << packet.tcpData->dstPort 
             << " (Flags: 0x" << std::hex << (int)packet.tcpData->flags << std::dec << ")\n";   
        if (packet.protocol.find("HTTP") != std::string::npos) {
            // Print HTTP details
            std::cout << "    [HTTP]" << " URL: " << packet.httpData->hostURL << "\n";
        };
    } 
    else if (packet.udpData.has_value()) {
        std::cout << "    [UDP]  Port " << packet.udpData->srcPort << " -> " << packet.udpData->dstPort << "\n";
    }

    std::cout << "--------------------------------------------------\n";
}

void Printer::printInterface(pcpp::PcapLiveDevice* interface) {
    if (interface == nullptr) {
        std::cerr << "Interface pointer is null." << "\n";
        return;
    }

    std::cout << "\n=== Interface Information ===\n";
    std::cout << "Name:       " << interface->getName() << "\n";
    
    std::cout << "Desc:       " << (interface->getDesc() != "" ? interface->getDesc() : "N/A") << "\n";
    std::cout << "MAC Addr:   " << interface->getMacAddress().toString() << "\n";
    std::cout << "Default GW: " << interface->getDefaultGateway().toString() << "\n";
    std::cout << "MTU:        " << interface->getMtu() << "\n";
    std::cout << "IPv4 Addr:  " << interface-> getIPv4Address() << "\n";

    std::cout << "=============================\n\n";
}
