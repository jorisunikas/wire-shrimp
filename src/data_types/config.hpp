#pragma once

#include <string>

/**
 * Holds CLI configuration passed into the program.
 */
struct Config {
    std::string interface; ///< Interface to capture packets on
    std::string filter; ///< Berkeley Packet Filter option for filtering packets
    int count = 100;    ///< Number of packets to capture, default value assumed
};
