#include "gameboy.h"
#include <iostream>

int main(int argc, char* argv[]){
    Gameboy gb;
    if(argc != 2){
        std::cout << "ROM filepath not provided: gameboy_emu.exe <filepath>" << std::endl;
    }

    gb.LoadCartridgeFromFile(argv[1]);
    gb.Boot();
    return 0;
}