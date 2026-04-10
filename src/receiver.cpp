#include "receiver.hpp"

using namespace std;

Receiver::~Receiver() {
    stop();
}

Receiver::Receiver(Config cfg) {
    config = cfg;
    
    // Find the device by IP or Name using the config string
    // find the interface by IP address
    device = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByIpOrName(config.interface);
    
    if (device == nullptr) {
        cerr << "Error: Interface '" << config.interface << "' not found." << endl;
        return;
    }

    if (!device->open()) {
        cerr << "Error: Could not open device." << endl;
        return;
    }

    cout << "Interface info:" << "\n" 
	     << "   Interface name:        " << device->getName() << "\n"
	     << "   Interface description: " << device->getDesc() << "\n"
         << "   MAC address:           " << device->getMacAddress() << "\n" 
	     << "   Default gateway:       " << device->getDefaultGateway() << "\n"
	     << "   Interface MTU:         " << device->getMtu() << "\n";
}

void Receiver::start() {
    if (device == nullptr || !device->isOpened()) {
        cerr << "Error: Device not initialized. Call Capture() first." << endl;
        return;
    }

    // Apply the BPF filter from config
    /*
    if (!config.filter.empty()) {
        if (!device->setFilter(config.filter)) {
            cerr << "Error: Invalid filter: " << config.filter << endl;
            return;
        }
    }
    */

    cout << "Starting capture on " << device->getName() << "..." << endl;
    isRunning = true;
    currentPacketCount = 0;

    // Start asynchronous capture; passing 'this' allows the static callback to access our instance
    device->startCapture(Receiver::onPacketArrives, this);
    while(isRunning){
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

void Receiver::stop() {
    if (isRunning && device != nullptr) {
        device->stopCapture();
        device->close();
        isRunning = false;
        cout << "Capture stopped." << endl;
    }
}

void Receiver::onPacket(pcpp::RawPacket* rawPacket) {
    currentPacketCount++;
    
    // Simple logic: print packet length
    cout << "Captured packet #" << currentPacketCount << " (" << rawPacket->getRawDataLen() << " bytes)" << endl;

    // Limit check
    if (config.count > 0 && currentPacketCount >= config.count) {
        cout << "Reached target packet count: " << config.count << endl;
        stop();
    }
}

// Static bridge to the onPacket member function
void Receiver::onPacketArrives(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* dev, void* cookie) {
    Receiver* instance = static_cast<Receiver*>(cookie);
    if (instance) {
        instance->onPacket(packet);
    }
}