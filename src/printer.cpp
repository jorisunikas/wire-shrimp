#include "printer.hpp"
#include <iomanip>
#include <iostream>

void Printer::printPacket(ParsedPacket packet) {
    // Only print TLS handshakes
    if (!packet.tlsData.has_value() || !packet.tlsData->isHandshake) {
        return;
    }

    if (!packet.valid) {
        std::cout << "[-] Invalid or malformed packet captured.\n";
        std::cout << "--------------------------------------------------\n";
        return;
    }

    std::cout << "[+] Packet Captured | Protocol: " << packet.protocol << "\n";

    std::cout << "    [ETH]  " << packet.ethData.srcMac << " -> "
              << packet.ethData.dstMac << " (Type: 0x" << std::hex
              << std::setfill('0') << std::setw(4) << packet.ethData.etherType
              << std::dec << ")\n";

    if (packet.IPv4Data.has_value()) {
        std::cout << "    [IPv4] " << packet.IPv4Data->srcIp << " -> "
                  << packet.IPv4Data->dstIp
                  << " (TTL: " << (int)packet.IPv4Data->ttl << ")\n";
    } else if (packet.IPv6Data.has_value()) {
        std::cout << "    [IPv6] " << packet.IPv6Data->srcIp << " -> "
                  << packet.IPv6Data->dstIp
                  << " (Hop Limit: " << (int)packet.IPv6Data->hopLimit << ")\n";
    }

    if (packet.tcpData.has_value()) {
        std::cout << "    [TCP]  Port " << packet.tcpData->srcPort << " -> "
                  << packet.tcpData->dstPort << " (Flags: 0x" << std::hex
                  << (int)packet.tcpData->flags << std::dec << ")\n";
        
        if (packet.httpData.has_value()) {
            // Print HTTP details
            std::cout << "    [HTTP]" << " URL: " << packet.httpData->hostURL << "\n";
        }
        
        else if (packet.tlsData.has_value()) {
            // Print TLS details
            std::cout << "    [TLS] " << "Version: " << packet.tlsData->recordHeader.versionStr << "\n";
            if(packet.tlsData->sniHeader.serverName != "") {
                std::cout << "    [TLS] " << "URL: " << packet.tlsData->sniHeader.serverName << "\n";
            }
        }
    } else if (packet.udpData.has_value()) {
        std::cout << "    [UDP]  Port " << packet.udpData->srcPort << " -> "
                  << packet.udpData->dstPort << "\n";
    }

    std::cout << "--------------------------------------------------\n";
}

void Printer::printInterface(pcpp::PcapLiveDevice *interface) {
    if (interface == nullptr) {
        std::cerr << "Interface pointer is null." << "\n";
        return;
    }

    std::cout << "\n=== Interface Information ===\n";
    std::cout << "Name:       " << interface->getName() << "\n";

    std::cout << "Desc:       "
              << (interface->getDesc() != "" ? interface->getDesc() : "N/A")
              << "\n";
    std::cout << "MAC Addr:   " << interface->getMacAddress().toString()
              << "\n";
    std::cout << "Default GW: " << interface->getDefaultGateway().toString()
              << "\n";
    std::cout << "MTU:        " << interface->getMtu() << "\n";
    std::cout << "IPv4 Addr:  " << interface->getIPv4Address() << "\n";
    std::cout << "IPv6 Addr:  " << interface->getIPv6Address() << "\n";

    std::cout << "=============================\n\n";
}
