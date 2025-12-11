#include <cartridge.h>
#include <stdexcept>
#include <iostream>

void Cartridge::Load(const std::vector<uint8_t>& data)
{
    if (data.size() < 0x150) {
        throw std::runtime_error("ROM file too small to be a valid Game Boy cartridge.");
    }

    rom = data;

    ParseHeader();
    AllocateRAM();
}

void Cartridge::ParseHeader()
{
    // Extract title (0x0134 - 0x0143)
    title.clear();
    for (int i = 0x0134; i <= 0x0143; i++) {
        if (rom[i] == 0) break;
        title.push_back(static_cast<char>(rom[i]));
    }

    // Cartridge type (0x0147)
    uint8_t type = rom[0x0147];

    hasBattery = false;

    switch (type) {
        case 0x00: mbcType = MBCType::None; break;

        case 0x01: mbcType = MBCType::MBC1; break;
        case 0x02: mbcType = MBCType::MBC1; break;
        case 0x03: mbcType = MBCType::MBC1; hasBattery = true; break;

        case 0x05: mbcType = MBCType::MBC2; break;
        case 0x06: mbcType = MBCType::MBC2; hasBattery = true; break;

        case 0x0F: mbcType = MBCType::MBC3; hasBattery = true; break;
        case 0x10: mbcType = MBCType::MBC3; hasBattery = true; break;
        case 0x11: mbcType = MBCType::MBC3; break;
        case 0x12: mbcType = MBCType::MBC3; break;
        case 0x13: mbcType = MBCType::MBC3; hasBattery = true; break;

        case 0x19: mbcType = MBCType::MBC5; break;
        case 0x1A: mbcType = MBCType::MBC5; break;
        case 0x1B: mbcType = MBCType::MBC5; hasBattery = true; break;
        case 0x1C: mbcType = MBCType::MBC5; break;
        case 0x1D: mbcType = MBCType::MBC5; break;
        case 0x1E: mbcType = MBCType::MBC5; hasBattery = true; break;

        default:
            mbcType = MBCType::Unknown;
            break;
    }

    // ROM size (0x0148)
    uint8_t romSizeCode = rom[0x0148];
    switch (romSizeCode) {
        case 0x00: romSizeBytes = 32 * 1024; break;
        case 0x01: romSizeBytes = 64 * 1024; break;
        case 0x02: romSizeBytes = 128 * 1024; break;
        case 0x03: romSizeBytes = 256 * 1024; break;
        case 0x04: romSizeBytes = 512 * 1024; break;
        case 0x05: romSizeBytes = 1024 * 1024; break;
        case 0x06: romSizeBytes = 2 * 1024 * 1024; break;
        case 0x07: romSizeBytes = 4 * 1024 * 1024; break;
        case 0x08: romSizeBytes = 8 * 1024 * 1024; break;
        default:
            romSizeBytes = rom.size(); // fallback
            break;
    }

    // RAM size (0x0149)
    uint8_t ramCode = rom[0x0149];
    switch (ramCode) {
        case 0x00: ramSizeBytes = 0; break;
        case 0x01: ramSizeBytes = 2 * 1024; break;
        case 0x02: ramSizeBytes = 8 * 1024; break;
        case 0x03: ramSizeBytes = 32 * 1024; break;
        case 0x04: ramSizeBytes = 128 * 1024; break;
        case 0x05: ramSizeBytes = 64 * 1024; break;
        default:
            ramSizeBytes = 0;
            break;
    }

    std::cout << "Cartridge loaded: " << title << "\n";
    std::cout << "MBC: " << (int)type << " (" << (int)ramCode << " RAM code)\n";
    std::cout << "ROM size: " << romSizeBytes / 1024 << " KB\n";
    std::cout << "RAM size: " << ramSizeBytes / 1024 << " KB\n";
}

void Cartridge::AllocateRAM()
{
    if (ramSizeBytes > 0)
        ram.resize(ramSizeBytes);
}
