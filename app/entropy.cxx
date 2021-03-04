#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <limits>
#include <cmath>

#include <fire-hpp/fire.hpp>

int fired_main(std::string file_path = fire::arg({"--file-path","-f"})) {
    std::ifstream file(file_path, std::fstream::binary);

    if (!file.is_open()) {
        std::cout << "Couldn't open file!" << std::endl;
        return 1;
    }
    file >> std::noskipws;

    std::map<char,int> freq;

    uint64_t total_bytes;

    char buf;
    while (file >> buf) {
        freq[buf]++;
        total_bytes++;
    }

    double entropy = 0;
    for (const auto& f : freq) {
        double p=(double)f.second/(double)total_bytes;
        entropy-=p*log2(p);
    }

    std::cout << "Entropy: " << entropy << "\n";
    std::cout << "Metric entropy: " << entropy/total_bytes << std::endl;

    file.close();
    return 0;
}

FIRE(fired_main, "Calculate file entropy")
