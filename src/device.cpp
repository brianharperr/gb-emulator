#include <device.h>
#include <fstream>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <iostream>

void Device::LoadCartridgeFromFile(const char* filepath){

    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error(std::string("Failed to open ROM file: ") + filepath);
    }

    // Read file into buffer
    std::vector<uint8_t> romData(
        (std::istreambuf_iterator<char>(file)),
        (std::istreambuf_iterator<char>())
    );

    if (romData.empty()) {
        throw std::runtime_error("ROM file is empty or unreadable.");
    }

    if (romData.size() < 32768) {
        std::cerr << "Warning: ROM is unusually small (" 
                  << romData.size() << " bytes)\n";
    }

    cartridge.Load(romData);

    std::cout << "Loaded ROM: " << filepath 
              << " (" << romData.size() << " bytes)" << std::endl;
}