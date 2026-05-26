#include "receiver.hpp"
#include "parser.hpp"
#include "printer.hpp"
#include <chrono>
#include <iostream>
#include <ostream>

// --- CONSTRUCTOR / DESTRUCTOR ---

Receiver::Receiver(Config cfg) {
    config = cfg;
    Printer::init(config);

    device = pcpp::PcapLiveDeviceList::getInstance().getDeviceByIpOrName(
        config.interface);

    if (device == nullptr) {
        std::cerr << "Error: Interface '" << config.interface << "' not found."
                  << "\n";
        return;
    }
    if (!device->open()) {
        std::cerr << "Error: Could not open device." << "\n";
        return;
    }

    Printer::printInterface(device);
}

Receiver::~Receiver() {
    device->close();
    Printer::cleanup();
}

// --- START / STOP ---

void Receiver::start() {
    if (device == nullptr || !device->isOpened()) {
        std::cerr << "Error: Device not initialized. Call Capture() first."
                  << "\n";
        return;
    }

    std::cout << "Starting capture on " << device->getName() << "...\n";

    startTime = std::chrono::steady_clock::now();
    currentPacketCount = 0;
    active = true;
    device->startCapture(Receiver::onPacketArrives, this);
    while (active) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    stop();
}

void Receiver::stop() {
    if (device != nullptr) {
        device->stopCapture();
        std::cout << "Capture stopped.\n";

        Printer::printTitle("Capture Report");
        Printer::out() << stats.getReport();
    }
}

// --- PACKET EVENTS ---

void Receiver::onPacketArrives(pcpp::RawPacket *packet,
                               pcpp::PcapLiveDevice *dev, void *cookie) {
    Receiver *instance = static_cast<Receiver *>(cookie);
    if (instance) {
        instance->onPacket(packet);
    }
}

void Receiver::onPacket(pcpp::RawPacket *rawPacket) {
    if (!active)
        return;

    // PACKET HANDLING
    ParsedPacket pp =
        Parser::parse({rawPacket->getRawData(),
                       static_cast<size_t>(rawPacket->getRawDataLen())});
    Printer::printPacket(pp, config.verbose);
    stats.add(pp);

    // CHECK STOP CONDITIONS
    currentPacketCount++;
    if (config.count > 0 && currentPacketCount >= config.count) {
        Printer::out() << "Reached target packet count: " << config.count << "\n";
        active = false;
    }
    if (config.timeout > 0) {
        auto elapsed = std::chrono::steady_clock::now() - startTime;
        auto seconds =
            std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
        if (seconds >= config.timeout) {
            Printer::out() << "Reached timeout: " << config.timeout << "s\n";
            active = false;
        }
    }
}
