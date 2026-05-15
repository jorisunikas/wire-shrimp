#include "printer.hpp"
#include <iostream>
#include <iomanip>

using namespace std;

void Printer::printPacket(ParsedPacket packet) {
    // Only print HTTP packets for now
    /*
    if(packet.protocol.find("HTTP") == string::npos) {
        return;
    }
    */

    if (!packet.valid) {
        cout << "[-] Invalid or malformed packet captured.\n";
        cout << "--------------------------------------------------\n";
        return;
    }

    cout << "[+] Packet Captured | Protocol: " << packet.protocol << "\n";

    cout << "    [ETH]  " << packet.ethData.srcMac << " -> " << packet.ethData.dstMac 
         << " (Type: 0x" << hex << setfill('0') << setw(4) << packet.ethData.etherType << dec << ")\n";

    if (packet.IPv4Data.has_value()) {
        cout << "    [IPv4] " << packet.IPv4Data->srcIp << " -> " << packet.IPv4Data->dstIp 
             << " (TTL: " << (int)packet.IPv4Data->ttl << ")\n";
    }

    if (packet.tcpData.has_value()) {
        cout << "    [TCP]  Port " << packet.tcpData->srcPort << " -> " << packet.tcpData->dstPort 
             << " (Flags: 0x" << hex << (int)packet.tcpData->flags << dec << ")\n";   
        if (packet.protocol.find("HTTP") != string::npos) {
            // Print HTTP details
            //cout << "    [HTTP]" << " " << packet.httpData->details << "\n";
        };
    } 
    else if (packet.udpData.has_value()) {
        cout << "    [UDP]  Port " << packet.udpData->srcPort << " -> " << packet.udpData->dstPort << "\n";
    }

    cout << "--------------------------------------------------\n";
}

void Printer::printInterface(pcpp::PcapLiveDevice* interface) {
    if (interface == nullptr) {
        cerr << "Interface pointer is null." << "\n";
        return;
    }

    cout << "\n=== Interface Information ===\n";
    cout << "Name:       " << interface->getName() << "\n";
    
    cout << "Desc:       " << (interface->getDesc() != "" ? interface->getDesc() : "N/A") << "\n";
    cout << "MAC Addr:   " << interface->getMacAddress().toString() << "\n";
    cout << "Default GW: " << interface->getDefaultGateway().toString() << "\n";
    cout << "MTU:        " << interface->getMtu() << "\n";
    cout << "IPv4 Addr:  " << interface-> getIPv4Address() << "\n";

    cout << "=============================\n\n";
}