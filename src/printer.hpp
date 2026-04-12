#pragma once

#include "packet.hpp"
#include <string>
#include <PcapLiveDevice.h>

using namespace std;

class Printer {
  public:
    static void printPacket(ParsedPacket packet);
    static void printInterface(pcpp::PcapLiveDevice*  interface);
};