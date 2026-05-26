#pragma once

#include "data_types/packet.hpp"
#include <PcapLiveDevice.h>

class Printer {
  public:
    static void printPacket(ParsedPacket packet);
    static void printInterface(pcpp::PcapLiveDevice *interface);
    static void printTitle(std::string title);
};
