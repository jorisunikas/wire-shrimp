#include "parser.hpp"

using namespace std;

// Static parser instances
unique_ptr<EthernetParser> Parser::ethernetParser = nullptr;
unique_ptr<IPv4Parser> Parser::ipv4Parser = nullptr;
unique_ptr<TCPParser> Parser::tcpParser = nullptr;
unique_ptr<UDPParser> Parser::udpParser = nullptr;
unique_ptr<HTTPParser> Parser::httpParser = nullptr;

unique_ptr<Indexer> Parser::indexer = nullptr;

void Parser::initialize() {
    if (ethernetParser == nullptr) {
        // L2
        ethernetParser = make_unique<EthernetParser>();
        // L3
        ipv4Parser = make_unique<IPv4Parser>();
        // L4
        tcpParser = make_unique<TCPParser>();
        udpParser = make_unique<UDPParser>();
        httpParser = make_unique<HTTPParser>();
    }

    Parser::indexer = make_unique<Indexer>();
}

ParsedPacket Parser::parse(RawPacket rp) {
    ParsedPacket pp;
    int offset = 0;

    // Initialize parsers on first use
    initialize();

    // ===== LAYER 2: ETHERNET =====
    if (ethernetParser->isValid(rp)) {
        pp.valid = true;
        pp.ethData = ethernetParser->parse(rp);
    }
    else{
        pp.valid = false;
        return pp; // Invalid packet (too short for Ethernet header)
    }

    // ===== LAYER 3: IPv4 =====

    if (ipv4Parser->isValid(rp)) {
        pp.IPv4Data = ipv4Parser->parse(rp);
    } else {
        return pp; // Not IPv4, stop parsing further
    }

    // ===== LAYER 4: TCP/UDP =====
    
    /*
     * Parse transport layer based on IPv4 protocol field
     * 1 = ICMP, 6 = TCP, 17 = UDP
     */
    
    // TCP
    if (pp.IPv4Data->protocol == 6) {
        
        if (tcpParser->isValid(rp)) {
            pp.tcpData = tcpParser->parse(rp);
            pp.protocol.append(" TCP");
        }

        // HTTP
        if (pp.tcpData->srcPort == 80 || pp.tcpData->dstPort == 80) {
            // Parsing
            if (httpParser->isValid(rp)) {
                pp.protocol.append(" HTTP");
                pp.httpData = httpParser->parse(rp, *indexer);
            }

            //Extracting URL
            if(indexer->getURL(pp.IPv4Data->srcIp) != "") {
                pp.httpData->hostURL = indexer->getURL(pp.IPv4Data->srcIp);
            } else if (pp.httpData) {
                pp.httpData->hostURL = httpParser->extractHostURL(*pp.httpData);
                if(pp.httpData->hostURL != "") {
                    indexer->addURL(pp.httpData->hostURL, pp.IPv4Data->dstIp);
                }
            }
        }
    } 
    // UDP
    else if (pp.IPv4Data->protocol == 17) {
        if (udpParser->isValid(rp)) {
            pp.udpData = udpParser->parse(rp);
            pp.protocol.append(" UDP");
        }
    } 
    // ICMP (no parsing implemented)
    else if (pp.IPv4Data->protocol == 1) {
        
        pp.protocol.append(" ICMP");
    }

    // ===== LAYER 6: HTTP/HTTPS/DNS =====

    
    return pp;
}
