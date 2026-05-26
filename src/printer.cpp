#include "printer.hpp"
#include <iomanip>
#include <iostream>

std::ofstream Printer::fileStream;
std::ostream *Printer::outStream = &std::cout;

void Printer::init(const Config &config) {
    if (!config.outputFile.empty()) {
        fileStream.open(config.outputFile);
        if (fileStream.is_open()) {
            outStream = &fileStream;
        } else {
            std::cerr << "Warning: Could not open '" << config.outputFile
                      << "', falling back to stdout.\n";
            outStream = &std::cout;
        }
    }
}

void Printer::cleanup() {
    if (fileStream.is_open()) {
        fileStream.close();
    }
    outStream = &std::cout;
}

std::ostream &Printer::out() { return *outStream; }

void Printer::printPacket(const ParsedPacket &packet, bool verbose) {
    if (verbose) {
        printPacketVerbose(packet);
    } else {
        printPacketCompact(packet);
    }
}

void Printer::printPacketCompact(const ParsedPacket &packet) {
    if (!packet.valid) {
        out() << "[-] Invalid packet\n";
        return;
    }

    std::string src, dst, extra;

    if (packet.IPv4Data.has_value()) {
        src = packet.IPv4Data->srcIp;
        dst = packet.IPv4Data->dstIp;
    } else if (packet.IPv6Data.has_value()) {
        src = packet.IPv6Data->srcIp;
        dst = packet.IPv6Data->dstIp;
    } else {
        src = packet.ethData.srcMac;
        dst = packet.ethData.dstMac;
    }

    if (packet.tcpData.has_value()) {
        src += ":" + std::to_string(packet.tcpData->srcPort);
        dst += ":" + std::to_string(packet.tcpData->dstPort);

        if (packet.httpData.has_value()) {
            extra = "HTTP: " + packet.httpData->hostURL;
        } else if (packet.tlsData.has_value() &&
                   !packet.tlsData->sniHeader.serverName.empty()) {
            extra = "TLS: " + packet.tlsData->sniHeader.serverName;
        }
    } else if (packet.udpData.has_value()) {
        src += ":" + std::to_string(packet.udpData->srcPort);
        dst += ":" + std::to_string(packet.udpData->dstPort);

        if (packet.dnsData.has_value()) {
            extra = "DNS: " + packet.dnsData->queryName;
        }
    }

    out() << "[" << packet.protocol << "] " << src
          << " -> " << dst;
    if (!extra.empty()) {
        out() << "  (" << extra << ")";
    }
    out() << "\n";
}

void Printer::printPacketVerbose(const ParsedPacket &packet) {
    auto &o = out();

    if (!packet.valid) {
        o << "[-] Invalid or malformed packet captured.\n";
        o << "--------------------------------------------------\n";
        return;
    }

    o << "[+] Packet Captured | Protocol: " << packet.protocol << "\n";

    o << "    [ETH]  " << packet.ethData.srcMac << " -> "
      << packet.ethData.dstMac << " (Type: 0x" << std::hex << std::setfill('0')
      << std::setw(4) << packet.ethData.etherType << std::dec << ")\n";

    if (packet.IPv4Data.has_value()) {
        o << "    [IPv4] " << packet.IPv4Data->srcIp << " -> "
          << packet.IPv4Data->dstIp << " (TTL: " << (int)packet.IPv4Data->ttl
          << ")\n";
    } else if (packet.IPv6Data.has_value()) {
        o << "    [IPv6] " << packet.IPv6Data->srcIp << " -> "
          << packet.IPv6Data->dstIp
          << " (Hop Limit: " << (int)packet.IPv6Data->hopLimit << ")\n";
    }

    if (packet.tcpData.has_value()) {
        o << "    [TCP]  Port " << packet.tcpData->srcPort << " -> "
          << packet.tcpData->dstPort << " (Flags: 0x" << std::hex
          << (int)packet.tcpData->flags << std::dec << ")\n";

        if (packet.httpData.has_value()) {
            o << "    [HTTP] URL: " << packet.httpData->hostURL << "\n";
        } else if (packet.tlsData.has_value()) {
            o << "    [TLS]  Version: "
              << packet.tlsData->recordHeader.versionStr << "\n";
            if (!packet.tlsData->sniHeader.serverName.empty()) {
                o << "    [TLS]  URL: " << packet.tlsData->sniHeader.serverName
                  << "\n";
            }
        }
    } else if (packet.udpData.has_value()) {
        o << "    [UDP]  Port " << packet.udpData->srcPort << " -> "
          << packet.udpData->dstPort << "\n";

        if (packet.dnsData.has_value()) {
            o << "    [DNS]  Query: " << packet.dnsData->queryName << "\n";
        }
    }

    o << "--------------------------------------------------\n";
}

void Printer::printInterface(pcpp::PcapLiveDevice *interface) {
    if (interface == nullptr) {
        std::cerr << "Interface pointer is null.\n";
        return;
    }

    auto &o = out();
    o << "\n=== Interface Information ===\n";
    o << "Name:       " << interface->getName() << "\n";
    o << "Desc:       "
      << (interface->getDesc() != "" ? interface->getDesc() : "N/A") << "\n";
    o << "MAC Addr:   " << interface->getMacAddress().toString() << "\n";
    o << "Default GW: " << interface->getDefaultGateway().toString() << "\n";
    o << "MTU:        " << interface->getMtu() << "\n";
    o << "IPv4 Addr:  " << interface->getIPv4Address() << "\n";
    o << "IPv6 Addr:  " << interface->getIPv6Address() << "\n";
    o << "=============================\n\n";
}

void Printer::printTitle(const std::string &title) {
    out() << "\n===== " << title << " =====\n";
}
