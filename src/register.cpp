#include "register.h"

void ByteRegister::Set(const uint8_t value){ val = value; }
void ByteRegister::Increment(){ val += 1; }
void ByteRegister::Decrement(){ val -= 1; }

void WordRegister::Set(const uint16_t value){ val = value;}
void WordRegister::Increment(){ val += 1; }
void WordRegister::Decrement(){ val -= 1; }

void FlagRegister::Set(const uint8_t value){ val = value & 0xF0; }
void FlagRegister::SetZero(bool set){ set_bit(7, set); }
void FlagRegister::SetSubtract(bool set){ set_bit(6, set); }
void FlagRegister::SetHalfCarry(bool set){ set_bit(5, set); }
void FlagRegister::SetCarry(bool set){ set_bit(4, set); }

bool FlagRegister::Zero(){ return check_bit(val, 7); }
bool FlagRegister::Subtract(){ return check_bit(val, 6); }
bool FlagRegister::HalfCarry(){ return check_bit(val, 5); }
bool FlagRegister::Carry(){ return check_bit(val, 4); }