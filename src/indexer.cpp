#include "indexer.hpp"

#include <iostream>

//#include <bits/stdc++.h>
//
std::string trim(std::string str) {
	const char* typeOfWhitespaces = " \t\n\r\f\v";
	str.erase(str.find_last_not_of(typeOfWhitespaces) + 1);
	str.erase(0,str.find_first_not_of(typeOfWhitespaces));
	return str;
}

std::unordered_map<std::string, std::string> Indexer::ipMap;

void Indexer::addURL(std::string ip, std::string url) {
    std::cout << "Adding URL to indexer: " << ip << " -> " << url << "\n"
              << "--------------------------------------------------\n";
    ipMap[trim(ip)] = trim(url);
}

std::string Indexer::getURL(std::string ip){
    if (ipMap.find(trim(ip)) != ipMap.end()) {
        return ipMap.at(trim(ip));
    }
    return "";
}
