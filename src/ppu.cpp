#include "ppu.h"

PPU::PPU(Gameboy& gb) : gb(gb)
{

}

void PPU::Tick(int cyc)
{
    cycles += cyc;

    switch (mode) {
    case OAM:
        if (cyc >= 80) {
            cycles -= 80;
            mode = DRAW;
        }
        break;

    case DRAW:
        if (cycles >= 172) {
            cycles -= 172;
            DrawScanline();
            mode = HBLANK;
        }
        break;

    case HBLANK:
        if (cycles >= 204) {
            cycles -= 204;
            ly++;
            if (ly == 144) {
                mode = VBLANK;
            } else {
                mode = OAM;
            }
        }
        break;

    case VBLANK:
        if (cycles >= 456) {
            cycles -= 456;
            ly++;
            if (ly > 153) {
                ly = 0;
                mode = OAM;
            }
        }
        break;
    }
}

void PPU::DrawBackgroundLine()
{
    bool use_tile_set_zero = WindowTileMap();
    bool use_tile_map_zero = !BackgroundTileMap();

    for(unsigned int x = 0; x < GAMEBOY_WIDTH; x++)
    {

    }
}

void PPU::DrawWindowLine()
{

}

void PPU::DrawScanline()
{
    if(!DisplayEnabled()) { return; }

    if(BackgroundEnabled()) { DrawBackgroundLine(); }

    if(WindowEnabled()) { DrawWindowLine(); }
}