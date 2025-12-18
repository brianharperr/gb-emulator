#pragma once

#include <array>

class PPU;
class SM83;
class Cartridge;

class Gameboy{
public:
    Gameboy();
    void LoadCartridgeFromFile(const char* filepath);
    void Boot();
    uint8_t ReadMem(uint16_t addr);
    void WriteMem(uint16_t addr, uint8_t data);
    
private:
    PPU* ppu;
    SM83* cpu;
    Cartridge* cartridge;
    std::array<uint8_t, 0x2000> wram{};   // Work RAM (C000-DFFF)
    std::array<uint8_t, 0x2000> vram{};   // Video RAM (8000-9FFF)
    std::array<uint8_t, 0xA0> oam{};      // Sprite attribute table (FE00-FE9F)
    std::array<uint8_t, 0x80> hram{};     // High RAM (FF80-FFFE)
    std::array<uint8_t, 0x80> io{};       // IO registers (FF00-FF7F)
    uint8_t ie{};                         // Interrupt Enable Register (FFFF)
};