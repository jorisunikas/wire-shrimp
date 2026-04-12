#include "printer.hpp"
#include <iostream>
#include <iomanip>

using namespace std;

void Printer::printPacket(ParsedPacket packet) {
    if (!packet.valid) {
        cout << "[-] Invalid or malformed packet captured.\n";
        cout << "--------------------------------------------------\n";
        return;
    }

    cout << "[+] Packet Captured | Protocol: " << packet.protocol << "\n";

    cout << "    [ETH]  " << packet.eth.srcMac << " -> " << packet.eth.dstMac 
         << " (Type: 0x" << hex << setfill('0') << setw(4) << packet.eth.etherType << dec << ")\n";

    if (packet.ip.has_value()) {
        cout << "    [IPv4] " << packet.ip->srcIp << " -> " << packet.ip->dstIp 
             << " (TTL: " << (int)packet.ip->ttl << ")\n";
    }

    if (packet.tcp.has_value()) {
        cout << "    [TCP]  Port " << packet.tcp->srcPort << " -> " << packet.tcp->dstPort 
             << " (Flags: 0x" << hex << (int)packet.tcp->flags << dec << ")\n";
    } 
    else if (packet.udp.has_value()) {
        cout << "    [UDP]  Port " << packet.udp->srcPort << " -> " << packet.udp->dstPort << "\n";
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