#include <iostream>
#include <string>
#include <PcapLiveDeviceList.h>
#include <PcapLiveDevice.h>
#include <PcapFilter.h>
#include "config.hpp"

using namespace std;

class Receiver {
private:
    // Fields
    Config config;
    pcpp::PcapLiveDevice* device;
    int currentPacketCount;
    bool isRunning;

    // Static callback for PcapPlusPlus
    static void onPacketArrives(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* dev, void* cookie);

public:
    Receiver(Config cfg);
    ~Receiver();

    // Methods
    void start();
    void stop();
    void onPacket(pcpp::RawPacket* packet);
};