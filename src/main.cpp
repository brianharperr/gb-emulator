#include <device.h>
#include <iostream>

int main(int argc, char** argv){
    Device d;
    
    if(argc != 2){
        std::cout << "ROM filepath not provided: gameboy_emu.exe <filepath>" << std::endl;
    }

    d.LoadCartridgeFromFile(argv[1]);

    return 0;
}