#include "config.hpp"
#include "receiver.hpp"

#include "getopt.h"
#include <getopt.h>
#include <iostream>
#include <string>

void printHelp(const char *name) {
    std::cout << "Usage:\n" << name << " [OPTIONS]\n";
    std::cout << "\nOptions:\n"
              << "  -i, --interface=<iface> Network interface to capture on "
                 "(e.g. en0, eth0)\n"
              << "  -n, --count=<count>     Number of packets to capture "
                 "(default: 100)\n"
              << "  -f, --filter=<filter>   BPF filter string (e.g. \"tcp\", "
                 "\"port 80\")\n"
              << "  -h, --help              Prints this message\n";
}

int main(int argc, char *argv[]) {
    Config config;

    static struct option long_opts[] = {
        {"interface", required_argument, 0, 'i'},
        {"count", required_argument, 0, 'n'},
        {"filter", required_argument, 0, 'f'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}};

    int opt;
    while ((opt = getopt_long(argc, argv, "i:n:f:h", long_opts, nullptr)) !=
           -1) {
        switch (opt) {
        case 'i':
            config.interface = optarg;
            break;
        case 'n':
            config.count = std::stoi(optarg);
            break;
        case 'f':
            config.filter = optarg;
            break;
        case 'h':
            printHelp(argv[0]);
            return 0;
        default:
            printHelp(argv[0]);
            return 1;
        }
    }

    if (config.interface.empty()) {
        std::cerr << "Error: -i <interface> is required\n";
        printHelp(argv[0]);
        return 1;
    }


    
    Receiver receiver(config);
    receiver.start();

    return 0;
}
