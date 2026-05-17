#pragma once
#include "data_types/config.hpp"

#include <PcapFilter.h>
#include <PcapLiveDevice.h>
#include <PcapLiveDeviceList.h>

class Receiver {
  private:
    // Fields
    Config config;
    pcpp::PcapLiveDevice *device;

    int currentPacketCount;
    bool active;

    // Static callback for PcapPlusPlus
    static void onPacketArrives(pcpp::RawPacket *packet,
                                pcpp::PcapLiveDevice *dev, void *cookie);

  public:
    Receiver(Config cfg);
    Receiver(Config cfg, std::vector<pcpp::RawPacket *> &rawArray);
    ~Receiver();

    // Methods
    void start();
    void stop();
    void onPacket(pcpp::RawPacket *packet);
};
