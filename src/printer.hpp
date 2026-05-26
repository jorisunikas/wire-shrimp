#pragma once

#include "data_types/config.hpp"
#include "data_types/packet.hpp"

#include <PcapLiveDevice.h>
#include <fstream>
#include <ostream>

class Printer {
  public:
    static void init(const Config &config);
    static void cleanup();
    static std::ostream &out();

    static void printPacket(const ParsedPacket &packet, bool verbose);
    static void printInterface(pcpp::PcapLiveDevice *interface);
    static void printTitle(const std::string &title);

  private:
    static void printPacketVerbose(const ParsedPacket &packet);
    static void printPacketCompact(const ParsedPacket &packet);

    static std::ofstream fileStream;
    static std::ostream *outStream;
};
