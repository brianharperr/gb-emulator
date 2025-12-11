#pragma once
#include <cstdint>
#include <vector>
#include <string>

enum class MBCType {
    None,
    MBC1,
    MBC2,
    MBC3,
    MBC5,
    Unknown
};

class Cartridge {
public:
    void Load(const std::vector<uint8_t>& data);

    const std::string& GetTitle() const { return title; }
    MBCType GetMBCType() const { return mbcType; }

    const std::vector<uint8_t>& GetROM() const { return rom; }
    std::vector<uint8_t>& GetRAM() { return ram; }

    bool HasRAM() const { return !ram.empty(); }
    bool HasBattery() const { return hasBattery; }

private:
    void ParseHeader();
    void AllocateRAM();

private:
    std::vector<uint8_t> rom;
    std::vector<uint8_t> ram;

    std::string title;

    MBCType mbcType = MBCType::Unknown;
    bool hasBattery = false;

    uint32_t romSizeBytes = 0;
    uint32_t ramSizeBytes = 0;
};
