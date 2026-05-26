#pragma once

#include <string>

/**
 * Holds CLI configuration passed into the program.
 */
struct Config {
    std::string interface; ///< Interface to capture packets on
    std::string filter; ///< Berkeley Packet Filter option for filtering packets
    std::string outputFile; ///< Where output will be written
    int count = 0;   ///< Number of packets to capture, default value assumed
    int timeout = 0; ///< Time of capture, seconds
    bool verbose = false; ///< should the entire info be displayed
};
