#include "receiver.hpp"

using namespace std;


// --- CONSTRUCTOR / DESTRUCTOR ---

Receiver::Receiver(Config cfg) {
    config = cfg;
    
    // Find the device by IP or Name using the config string
    // find the interface by IP address
    device = pcpp::PcapLiveDeviceList::getInstance().getDeviceByIpOrName(config.interface);
    
    if (device == nullptr) {
        cerr << "Error: Interface '" << config.interface << "' not found." << endl;
        return;
    }
    if (!device->open()) {
        cerr << "Error: Could not open device." << endl;
        return;
    }

    cout << "Interface found:" << "\n" 
	     << "   Interface name:  " << device->getName() << "\n"
	     << "   IPv4 address:    " << device->getIPv4Address() << "\n"
         << "   MAC address:     " << device->getMacAddress() << "\n" 
	     << "   Default gateway: " << device->getDefaultGateway() << "\n"
	     << "   Interface MTU:   " << device->getMtu() << "\n";
}

Receiver::~Receiver(){
    device->close();
}

// --- START / STOP ---

void Receiver::start() {
    if (device == nullptr || !device->isOpened()) {
        cerr << "Error: Device not initialized. Call Capture() first." << endl;
        return;
    }

    cout << "Starting capture on " << device->getName() << "..." << endl;

    // Start asynchronous capture; passing 'this' allows the static callback to access our instance
    currentPacketCount = 0;
    active = true;
    device->startCapture(Receiver::onPacketArrives, this);
    while(active){
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    stop();
}

void Receiver::stop() {
    if (device != nullptr) {
        device->stopCapture();
        cout << "Capture stopped." << endl;
    }
}

// --- PACKET EVENTS ---

// Static bridge to the onPacket member function
void Receiver::onPacketArrives(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* dev, void* cookie) {
    Receiver* instance = static_cast<Receiver*>(cookie);
    if (instance) {
        instance->onPacket(packet);
    }
}

void Receiver::onPacket(pcpp::RawPacket* rawPacket) {
    if (!active) return;

    currentPacketCount++;
    
    // Simple logic: print packet length
    cout << "Captured packet #" << currentPacketCount << " (" << rawPacket->getRawDataLen() << " bytes)" << endl;

    // Limit check
    if (config.count > 0 && currentPacketCount >= config.count) {
        active = false;
        cout << "Reached target packet count: " << config.count << endl;
    }
}

