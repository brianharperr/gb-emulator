#pragma once

#include <cstdint>

class SM83 {
public:
    void cycle();
private:
    //Registers
    int8_t a;
    int8_t f;
    int8_t b;
    int8_t c;
    int8_t d;
    int8_t e;
    int8_t h;
    int8_t l;
    int16_t sp;
    int16_t pc;
};