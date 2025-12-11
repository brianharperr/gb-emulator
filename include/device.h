#pragma once

#include "cpu.h"
#include "cartridge.h"

class Device{
public:
    void LoadCartridgeFromFile(const char* filepath);
private:
    SM83 cpu;
    Cartridge cartridge;
};