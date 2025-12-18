#include "gameboy.h"
#include "ppu.h"
#include "sm83.h"
#include "cartridge.h"
#include <fstream>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <iostream>

Gameboy::Gameboy()
{
    ppu = new PPU(*this);
    cpu = new SM83(*this);
    cartridge = new Cartridge();
}

void Gameboy::LoadCartridgeFromFile(const char* filepath){

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

    cartridge->Load(romData);

    std::cout << "Loaded ROM: " << filepath 
              << " (" << romData.size() << " bytes)" << std::endl;
}

void Gameboy::Boot()
{
    std::cout << "test" << std::endl;
    uint8_t cpuCycles;
    while(!cpu->IsHalted()){
        cpuCycles = cpu->Tick();
    }
    ppu->Tick(cpuCycles);
    // int STEPS = 10;
    // for(int i = 0; i < STEPS; i++){
    //     cpu.Tick();
    // }
}

uint8_t Gameboy::ReadMem(uint16_t addr)
{
    // ROM Bank 0 (0x0000 - 0x3FFF)
    if (addr <= 0x3FFF) {
        return cartridge->GetROM()[addr];
    }

    // Switchable ROM Bank (0x4000 - 0x7FFF) NEED TO IMPLEMENT
    else if (addr >= 0x4000 && addr <= 0x7FFF) {
        return cartridge->GetROM()[addr];
        //return rom[addr + (currentRomBank - 1) * 0x4000];
    }

    // Video RAM (0x8000 - 0x9FFF)
    else if (addr >= 0x8000 && addr <= 0x9FFF) {
        return vram[addr - 0x8000];
    }

    // External RAM (0xA000 - 0xBFFF)
    else if (addr >= 0xA000 && addr <= 0xBFFF) {
        return cartridge->GetRAM()[addr - 0xA000]; // Cartridge RAM
    }

    // Work RAM (0xC000 - 0xDFFF)
    else if (addr >= 0xC000 && addr <= 0xDFFF) {
        return wram[addr - 0xC000];
    }

    // Echo RAM (0xE000 - 0xFDFF)
    else if (addr >= 0xE000 && addr <= 0xFDFF) {
        return wram[addr - 0xE000]; // Mirrors C000-DDFF
    }

    // OAM (0xFE00 - 0xFE9F)
    else if (addr >= 0xFE00 && addr <= 0xFE9F) {
        return oam[addr - 0xFE00];
    }

    // Unusable memory (0xFEA0 - 0xFEFF)
    else if (addr >= 0xFEA0 && addr <= 0xFEFF) {
        return 0xFF; // Or open bus behavior
    }

    // IO Registers (0xFF00 - 0xFF7F)
    else if (addr >= 0xFF00 && addr <= 0xFF7F) {
        return io[addr - 0xFF00];
    }

    // High RAM (0xFF80 - 0xFFFE)
    else if (addr >= 0xFF80 && addr <= 0xFFFE) {
        return hram[addr - 0xFF80];
    }

    // Interrupt Enable Register (0xFFFF)
    else if (addr == 0xFFFF) {
        return ie;
    }

    // Default fallback
    return 0xFF;
}

void Gameboy::WriteMem(uint16_t addr, uint8_t data)
{
    // ROM (0x0000–0x7FFF)
    // Cannot be written directly — writes go to the MBC controller
    // if (addr <= 0x7FFF)
    // {
    //     cartridge.HandleWrite(addr, data);  // You must implement banking logic here
    //     return;
    // }

    // Video RAM (0x8000–0x9FFF)
    if (addr >= 0x8000 && addr <= 0x9FFF)
    {
        vram[addr - 0x8000] = data;
        return;
    }

    // External RAM (0xA000–0xBFFF)
    // if (addr >= 0xA000 && addr <= 0xBFFF)
    // {
    //     cartridge.WriteRAM(addr - 0xA000, data); // MBC-controlled RAM
    //     return;
    // }

    // Work RAM (0xC000–0xDFFF)
    if (addr >= 0xC000 && addr <= 0xDFFF)
    {
        wram[addr - 0xC000] = data;
        return;
    }

    // Echo RAM (0xE000–0xFDFF) mirrors C000–DDFF
    if (addr >= 0xE000 && addr <= 0xFDFF)
    {
        wram[addr - 0xE000] = data;
        return;
    }

    // OAM (0xFE00–0xFE9F)
    if (addr >= 0xFE00 && addr <= 0xFE9F)
    {
        oam[addr - 0xFE00] = data;
        return;
    }

    // Unusable memory (0xFEA0–0xFEFF)
    if (addr >= 0xFEA0 && addr <= 0xFEFF)
    {
        // Writes are ignored
        return;
    }

    // I/O Registers (0xFF00–0xFF7F)
    if (addr >= 0xFF00 && addr <= 0xFF7F)
    {
        io[addr - 0xFF00] = data;
        return;
    }

    // High RAM (0xFF80–0xFFFE)
    if (addr >= 0xFF80 && addr <= 0xFFFE)
    {
        hram[addr - 0xFF80] = data;
        return;
    }

    // Interrupt Enable (0xFFFF)
    if (addr == 0xFFFF)
    {
        ie = data;
        return;
    }
}