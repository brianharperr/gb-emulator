#pragma once

#include <cstdint>
#include "gameboy.h"
#include "register.h"
#include "window.h"
enum PPUMode { HBLANK=0, VBLANK=1, OAM=2, DRAW=3 };

const int CLOCK_RATE = 4194304;

class PPU{
public:
    PPU(Gameboy& gb);
    void Tick(int cycles);
    const uint32_t* GetFrameBuffer() const;
private:

    unsigned int GAMEBOY_WIDTH = 160;
    unsigned int GAMEBOY_HEIGHT = 144;
    unsigned int BG_MAP_SIZE = 256;

    Gameboy& gb;
    Window window;

    ByteRegister control;
    
    bool DisplayEnabled(){ return check_bit(control.Get(), 7); };
    bool WindowTileMap(){ return check_bit(control.Get(), 6); };
    bool WindowEnabled(){ return check_bit(control.Get(), 5); };
    bool BackgroundWindowTile(){ return check_bit(control.Get(), 4); };
    bool BackgroundTileMap(){ return check_bit(control.Get(), 3); };
    bool SpriteSize(){ return check_bit(control.Get(), 2); };
    bool SpritesEnabled(){ return check_bit(control.Get(), 1); };
    bool BackgroundEnabled(){ return check_bit(control.Get(), 0); };

    ByteRegister status;
    
    void DrawScanline();
    void DrawBackgroundLine();
    void DrawWindowLine();
    void DrawSpriteLine();

    unsigned int cycles;
    uint8_t ly;
    uint8_t lx;

    uint32_t frameBuffer[160 * 144];
    PPUMode mode = PPUMode::OAM;
};