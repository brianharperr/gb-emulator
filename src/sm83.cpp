#include "sm83.h"
#include "gameboy.h"
#include <iostream>
#include <iomanip>
SM83::SM83(Gameboy& gb) : gb(gb), a(0), b(0), c(0), d(0), e(0), h(0), l(0), f(0), sp(0xFFFE), pc(0x0100), halted(false)
{

}

uint8_t SM83::Tick()
{
    std::cout << std::hex << std::uppercase << std::setfill('0') << std::setw(4) << static_cast<int>(pc.Get());
    uint8_t opcode = GetByteFromPC();
    std::cout << " " << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<int>(opcode) << std::endl;
    const Opcode& op =
        (opcode == 0xCB)
        ? opcodeTableCB[GetByteFromPC()]
        : opcodeTable[opcode];

    (this->*op.handler)();
    return op.cycles;
}

uint8_t SM83::GetByteFromPC()
{
    uint8_t byte = gb.ReadMem(pc.Get());
    pc.Increment();
    return byte;
}

uint16_t SM83::GetWordFromPC()
{
    uint8_t lo = GetByteFromPC();
    uint8_t hi = GetByteFromPC();
    return (hi << 8) | lo;
}

void SM83::StackPush(uint16_t data)
{
    sp.Decrement();
    gb.WriteMem(sp.Get(), (data >> 8) & 0xFF);
    sp.Decrement();
    gb.WriteMem(sp.Get(), data & 0xFF);
}

uint16_t SM83::MakeWord(uint8_t lo, uint8_t hi)
{
    return (uint16_t(hi) << 8) | uint16_t(lo);
}

bool SM83::ConditionMet(Condition condition)
{
    bool should_branch;

    switch (condition) {
        case Condition::C:
            should_branch = f.Carry();
            break;
        case Condition::NC:
            should_branch = !f.Carry();
            break;
        case Condition::Z:
            should_branch = f.Zero();
            break;
        case Condition::NZ:
            should_branch = !f.Zero();
            break;
    }

    branched = should_branch;
    return should_branch;
}

void SM83::NOP(){}

void SM83::ADC(uint8_t data) 
{
    uint8_t carry = f.Carry() ? 1 : 0;
    uint16_t result = a.Get() + data + carry;

    f.SetZero((result & 0xFF) == 0);
    f.SetSubtract(false);
    f.SetHalfCarry(((a.Get() & 0xF) + (data & 0xF) + carry) > 0xF);
    f.SetCarry(result > 0xFF);

    a.Set(result & 0xFF);
}

void SM83::ADC_A_r8(ByteRegister& reg) { ADC(reg.Get()); }
void SM83::ADC_A_A(){ ADC_A_r8(a); }
void SM83::ADC_A_B(){ ADC_A_r8(b); }
void SM83::ADC_A_C(){ ADC_A_r8(c); }
void SM83::ADC_A_D(){ ADC_A_r8(d); }
void SM83::ADC_A_E(){ ADC_A_r8(e); }
void SM83::ADC_A_H(){ ADC_A_r8(h); }
void SM83::ADC_A_L(){ ADC_A_r8(l); }
void SM83::ADC_A_HL(){ ADC(gb.ReadMem(HL())); }
void SM83::ADC_A_n8(){ ADC(GetByteFromPC()); }

void SM83::ADD_A(uint8_t data)
{
    uint16_t result = a.Get() + data;

    f.SetZero((result & 0xFF) == 0);
    f.SetSubtract(false);
    f.SetHalfCarry(((a.Get() & 0xF) + (data & 0xF)) > 0xF);
    f.SetCarry(result > 0xFF);                           

    a.Set(result & 0xFF);
}

void SM83::ADD_A_r8(ByteRegister& reg) { ADD_A(reg.Get()); }
void SM83::ADD_A_A(){ ADD_A_r8(a); }
void SM83::ADD_A_B(){ ADD_A_r8(b); }
void SM83::ADD_A_C(){ ADD_A_r8(c); }
void SM83::ADD_A_D(){ ADD_A_r8(d); }
void SM83::ADD_A_E(){ ADD_A_r8(e); }
void SM83::ADD_A_H(){ ADD_A_r8(h); }
void SM83::ADD_A_L(){ ADD_A_r8(l); }
void SM83::ADD_A_HL(){ ADD_A(gb.ReadMem(HL())); }
void SM83::ADD_A_n8(){ ADD_A(GetByteFromPC()); }

void SM83::ADD_HL(uint16_t data)
{
    uint16_t hl = HL(); 
    uint32_t result = hl + data;

    f.SetSubtract(false);
    f.SetHalfCarry(((hl & 0x0FFF) + (data & 0x0FFF)) > 0x0FFF);
    f.SetCarry(result > 0xFFFF);

    h.Set((result >> 8) & 0xFF);
    l.Set(result & 0xFF);
}

void SM83::ADD_HL_BC(){ ADD_HL(BC()); }
void SM83::ADD_HL_DE(){ ADD_HL(DE()); }
void SM83::ADD_HL_HL(){ ADD_HL(HL()); }
void SM83::ADD_HL_SP(){ ADD_HL(sp.Get()); }

void SM83::ADD_SP_e8() 
{
    uint16_t s = sp.Get();
    int8_t e8 = GetByteFromPC();

    int result = static_cast<int>(s + e8);

    f.SetZero(false);
    f.SetSubtract(false);
    f.SetHalfCarry(((s ^ e8 ^ (result & 0xFFFF)) & 0x10) == 0x10);
    f.SetCarry(((s ^ e8 ^ (result & 0xFFFF)) & 0x100) == 0x100);

    sp.Set(static_cast<uint16_t>(result));
}

void SM83::AND_A_r8(ByteRegister& reg)
{
    a.Set(reg.Get() & a.Get());
}

void SM83::AND_A_A(){ AND_A_r8(a); }
void SM83::AND_A_B(){ AND_A_r8(b); }
void SM83::AND_A_C(){ AND_A_r8(c); }
void SM83::AND_A_D(){ AND_A_r8(d); }
void SM83::AND_A_E(){ AND_A_r8(e); }
void SM83::AND_A_H(){ AND_A_r8(h); }
void SM83::AND_A_L(){ AND_A_r8(l); }

void SM83::AND_A_HL()
{
    a.Set(gb.ReadMem(HL()) & a.Get()); 
}

void SM83::AND_A()
{
    a.Set(GetByteFromPC() & a.Get()); 
}

void SM83::BIT_r8(uint8_t bit, ByteRegister& reg)
{
    f.SetZero(!check_bit(reg.Get(), bit));
    f.SetSubtract(false);
    f.SetHalfCarry(true);
}

void SM83::BIT_HL(uint8_t bit, uint8_t value)
{
    f.SetZero(!check_bit(value, bit));
    f.SetSubtract(false);
    f.SetHalfCarry(true);
}

void SM83::BIT_0_A(){ BIT_r8(0, a); } // 0xCB 0x47
void SM83::BIT_1_A(){ BIT_r8(1, a); } // 0xCB 0x4F
void SM83::BIT_2_A(){ BIT_r8(2, a); } // 0xCB 0x57
void SM83::BIT_3_A(){ BIT_r8(3, a); } // 0xCB 0x5F
void SM83::BIT_4_A(){ BIT_r8(4, a); } // 0xCB 0x67
void SM83::BIT_5_A(){ BIT_r8(5, a); } // 0xCB 0x6F
void SM83::BIT_6_A(){ BIT_r8(6, a); } // 0xCB 0x77
void SM83::BIT_7_A(){ BIT_r8(7, a); } // 0xCB 0x7F

void SM83::BIT_0_B(){ BIT_r8(0, b); } // 0xCB 0x40
void SM83::BIT_1_B(){ BIT_r8(1, b); } // 0xCB 0x48
void SM83::BIT_2_B(){ BIT_r8(2, b); } // 0xCB 0x50
void SM83::BIT_3_B(){ BIT_r8(3, b); } // 0xCB 0x58
void SM83::BIT_4_B(){ BIT_r8(4, b); } // 0xCB 0x60
void SM83::BIT_5_B(){ BIT_r8(5, b); } // 0xCB 0x68
void SM83::BIT_6_B(){ BIT_r8(6, b); } // 0xCB 0x70
void SM83::BIT_7_B(){ BIT_r8(7, b); } // 0xCB 0x78

void SM83::BIT_0_C(){ BIT_r8(0, c); } // 0xCB 0x41
void SM83::BIT_1_C(){ BIT_r8(1, c); } // 0xCB 0x49
void SM83::BIT_2_C(){ BIT_r8(2, c); } // 0xCB 0x51
void SM83::BIT_3_C(){ BIT_r8(3, c); } // 0xCB 0x59
void SM83::BIT_4_C(){ BIT_r8(4, c); } // 0xCB 0x61
void SM83::BIT_5_C(){ BIT_r8(5, c); } // 0xCB 0x69
void SM83::BIT_6_C(){ BIT_r8(6, c); } // 0xCB 0x71
void SM83::BIT_7_C(){ BIT_r8(7, c); } // 0xCB 0x79

void SM83::BIT_0_D(){ BIT_r8(0, d); } // 0xCB 0x42
void SM83::BIT_1_D(){ BIT_r8(1, d); } // 0xCB 0x4A
void SM83::BIT_2_D(){ BIT_r8(2, d); } // 0xCB 0x52
void SM83::BIT_3_D(){ BIT_r8(3, d); } // 0xCB 0x5A
void SM83::BIT_4_D(){ BIT_r8(4, d); } // 0xCB 0x62
void SM83::BIT_5_D(){ BIT_r8(5, d); } // 0xCB 0x6A
void SM83::BIT_6_D(){ BIT_r8(6, d); } // 0xCB 0x72
void SM83::BIT_7_D(){ BIT_r8(7, d); } // 0xCB 0x7A


void SM83::BIT_0_E(){ BIT_r8(0, e); } // 0xCB 0x43
void SM83::BIT_1_E(){ BIT_r8(1, e); } // 0xCB 0x4B
void SM83::BIT_2_E(){ BIT_r8(2, e); } // 0xCB 0x53
void SM83::BIT_3_E(){ BIT_r8(3, e); } // 0xCB 0x5B
void SM83::BIT_4_E(){ BIT_r8(4, e); } // 0xCB 0x63
void SM83::BIT_5_E(){ BIT_r8(5, e); } // 0xCB 0x6B
void SM83::BIT_6_E(){ BIT_r8(6, e); } // 0xCB 0x73
void SM83::BIT_7_E(){ BIT_r8(7, e); } // 0xCB 0x7B

void SM83::BIT_0_H(){ BIT_r8(0, h); } // 0xCB 0x44
void SM83::BIT_1_H(){ BIT_r8(1, h); } // 0xCB 0x4C
void SM83::BIT_2_H(){ BIT_r8(2, h); } // 0xCB 0x54
void SM83::BIT_3_H(){ BIT_r8(3, h); } // 0xCB 0x5C
void SM83::BIT_4_H(){ BIT_r8(4, h); } // 0xCB 0x64
void SM83::BIT_5_H(){ BIT_r8(5, h); } // 0xCB 0x6C
void SM83::BIT_6_H(){ BIT_r8(6, h); } // 0xCB 0x74
void SM83::BIT_7_H(){ BIT_r8(7, h); } // 0xCB 0x7C


void SM83::BIT_0_L(){ BIT_r8(0, l); } // 0xCB 0x45
void SM83::BIT_1_L(){ BIT_r8(1, l); } // 0xCB 0x4D
void SM83::BIT_2_L(){ BIT_r8(2, l); } // 0xCB 0x55
void SM83::BIT_3_L(){ BIT_r8(3, l); } // 0xCB 0x5D
void SM83::BIT_4_L(){ BIT_r8(4, l); } // 0xCB 0x65
void SM83::BIT_5_L(){ BIT_r8(5, l); } // 0xCB 0x6D
void SM83::BIT_6_L(){ BIT_r8(6, l); } // 0xCB 0x75
void SM83::BIT_7_L(){ BIT_r8(7, l); } // 0xCB 0x7D

void SM83::BIT_0_HL(){ BIT_HL(0, gb.ReadMem(HL())); } // 0xCB 0x46
void SM83::BIT_1_HL(){ BIT_HL(1, gb.ReadMem(HL())); } // 0xCB 0x4E
void SM83::BIT_2_HL(){ BIT_HL(2, gb.ReadMem(HL())); } // 0xCB 0x56
void SM83::BIT_3_HL(){ BIT_HL(3, gb.ReadMem(HL())); } // 0xCB 0x5E
void SM83::BIT_4_HL(){ BIT_HL(4, gb.ReadMem(HL())); } // 0xCB 0x66
void SM83::BIT_5_HL(){ BIT_HL(5, gb.ReadMem(HL())); } // 0xCB 0x6E
void SM83::BIT_6_HL(){ BIT_HL(6, gb.ReadMem(HL())); } // 0xCB 0x76
void SM83::BIT_7_HL(){ BIT_HL(7, gb.ReadMem(HL())); } // 0xCB 0x7E

void SM83::CALL()
{
    uint16_t addr = GetWordFromPC();
    StackPush(pc.Get());
    pc.Set(addr);
}

void SM83::CALL_CONDITION(Condition condition)
{
    if(ConditionMet(condition)){
        CALL();
    }else{
        GetWordFromPC();
    }
}

void SM83::CALL_NZ(){ CALL_CONDITION(Condition::NZ); }
void SM83::CALL_Z(){ CALL_CONDITION(Condition::Z); }
void SM83::CALL_NC(){ CALL_CONDITION(Condition::NC); }
void SM83::CALL_C(){ CALL_CONDITION(Condition::C); }

void SM83::CCF()
{
    f.SetSubtract(false);
    f.SetHalfCarry(false);
    f.SetCarry(!f.Carry());
}

void SM83::CP_A(uint8_t value)
{
    uint8_t reg = a.Get();
    uint8_t result = static_cast<uint8_t>(reg - value);

    f.SetZero(result == 0);
    f.SetSubtract(true);
    f.SetHalfCarry(((reg & 0xf) - (value & 0xf)) < 0);
    f.SetCarry(reg < value);
}

void SM83::CP_A_A(){ CP_A(a.Get()); }
void SM83::CP_A_B(){ CP_A(b.Get()); }
void SM83::CP_A_C(){ CP_A(c.Get()); }
void SM83::CP_A_D(){ CP_A(d.Get()); }
void SM83::CP_A_E(){ CP_A(e.Get()); }
void SM83::CP_A_H(){ CP_A(h.Get()); }
void SM83::CP_A_L(){ CP_A(l.Get()); }
void SM83::CP_A_n8(){ CP_A(GetByteFromPC()); }
void SM83::CP_A_HL(){ CP_A(gb.ReadMem(HL())); }

void SM83::CPL()
{
    a.Set(~a.Get());
    f.SetSubtract(true);
    f.SetHalfCarry(true);
}

void SM83::DAA()
{
    uint8_t aVal = a.Get();

    uint16_t correction = f.Carry()
        ? 0x60
        : 0x00;

    if (f.HalfCarry() || (!f.Subtract() && ((aVal & 0x0F) > 9))) {
        correction |= 0x06;
    }

    if (f.Carry() || (!f.Subtract() && (aVal > 0x99))) {
        correction |= 0x60;
    }

    if (f.Subtract()) {
        aVal = static_cast<uint8_t>(aVal - correction);
    } else {
        aVal = static_cast<uint8_t>(aVal + correction);
    }

    if (((correction << 2) & 0x100) != 0) {
        f.SetCarry(true);
    }

    f.SetHalfCarry(false);
    f.SetZero(aVal == 0);

    a.Set(static_cast<uint8_t>(aVal));
}

void SM83::DEC(ByteRegister& reg)
{
    reg.Decrement();

    f.SetZero(reg.Get() == 0);
    f.SetSubtract(true);
    f.SetHalfCarry((reg.Get() & 0x0F) == 0x0F);
}

void SM83::DEC_A(){ DEC(a); }
void SM83::DEC_B(){ DEC(b); }
void SM83::DEC_C(){ DEC(c); }
void SM83::DEC_D(){ DEC(d); }
void SM83::DEC_E(){ DEC(e); }
void SM83::DEC_H(){ DEC(h); }
void SM83::DEC_L(){ DEC(l); }
void SM83::DEC_BC()
{
    uint16_t lo = c.Get();
    uint16_t hi = b.Get();

    if (lo == 0) {
        lo = 0xFF;
        hi = (hi - 1) & 0xFF;
    } else {
        lo--;
    }

    b.Set(hi);
    c.Set(lo);
}

void SM83::DEC_DE()
{
    uint16_t lo = e.Get();
    uint16_t hi = d.Get();

    if (lo == 0) {
        lo = 0xFF;
        hi = (hi - 1) & 0xFF;
    } else {
        lo--;
    }

    d.Set(hi);
    e.Set(lo);
}

void SM83::DEC_HL_addr()
{
    uint8_t val = gb.ReadMem(HL());
    uint8_t result = val - 1;
    
    f.SetZero(result == 0);
    f.SetSubtract(true);
    f.SetHalfCarry((val & 0x0F) == 0x00);

    gb.WriteMem(HL(), result);
}


void SM83::DEC_HL()
{
    uint16_t lo = l.Get();
    uint16_t hi = h.Get();

    if (lo == 0) {
        lo = 0xFF;
        hi = (hi - 1) & 0xFF;
    } else {
        lo--;
    }

    h.Set(hi);
    l.Set(lo);
}

void SM83::DEC_SP()
{ 
    sp.Decrement();
}

void SM83::DI(){ interrupts_enabled = false; }
void SM83::EI(){ interrupts_enabled = true; }

void SM83::HALT() { halted = true; }

void SM83::INC(ByteRegister& reg)
{
    reg.Increment();

    f.SetZero(reg.Get() == 0);
    f.SetSubtract(false);
    f.SetHalfCarry((reg.Get() & 0x0F) == 0x00);
}

void SM83::INC_A(){ INC(a); }
void SM83::INC_B(){ INC(b); }
void SM83::INC_C(){ INC(c); }
void SM83::INC_D(){ INC(d); }
void SM83::INC_E(){ INC(e); }
void SM83::INC_H(){ INC(h); }
void SM83::INC_L(){ INC(l); }
void SM83::INC_BC()
{
    uint16_t lo = c.Get();
    uint16_t hi = b.Get();

    if (lo == 0xFF) {
        lo = 0x00;
        hi = (hi + 1) & 0xFF;
    } else {
        lo++;
    }

    b.Set(hi);
    c.Set(lo);
}


void SM83::INC_DE()
{
    uint16_t lo = e.Get();
    uint16_t hi = d.Get();

    if (lo == 0xFF) {
        lo = 0x00;
        hi = (hi + 1) & 0xFF;
    } else {
        lo++;
    }

    d.Set(hi);
    e.Set(lo);
}


void SM83::INC_HL_addr()
{
    uint8_t val = gb.ReadMem(HL());
    uint8_t result = val + 1;

    f.SetZero(result == 0);
    f.SetSubtract(false);
    f.SetHalfCarry((val & 0x0F) == 0x0F);

    gb.WriteMem(HL(), result);
}


void SM83::INC_HL()
{
    uint16_t lo = l.Get();
    uint16_t hi = h.Get();

    if (lo == 0xFF) {
        lo = 0x00;
        hi = (hi + 1) & 0xFF;
    } else {
        lo++;
    }

    h.Set(hi);
    l.Set(lo);
}


void SM83::INC_SP()
{ 
    sp.Increment();
}

void SM83::JP()
{
    pc.Set(GetWordFromPC());
}

void SM83::JP_CONDITION(Condition condition)
{
    if(ConditionMet(condition)){
        JP();
    }else{
        GetWordFromPC();
    }
}

void SM83::JP_NZ(){ JP_CONDITION(Condition::NZ); }
void SM83::JP_Z(){ JP_CONDITION(Condition::Z); }
void SM83::JP_NC(){ JP_CONDITION(Condition::NC); }
void SM83::JP_C(){ JP_CONDITION(Condition::C); }

void SM83::JP_HL()
{
    pc.Set(HL());
}

void SM83::JR()
{
    int8_t offset = GetByteFromPC();

    uint16_t old_pc = pc.Get();

    uint16_t new_pc = static_cast<uint16_t>(old_pc + offset);
    pc.Set(new_pc);
}

void SM83::JR_CONDITION(Condition condition)
{
    if(ConditionMet(condition)){
        JR();
    }else{
        GetWordFromPC();
    }
}

void SM83::JR_NZ(){ JR_CONDITION(Condition::NZ); }
void SM83::JR_Z(){ JR_CONDITION(Condition::Z); }
void SM83::JR_NC(){ JR_CONDITION(Condition::NC); }
void SM83::JR_C(){ JR_CONDITION(Condition::C); }

void SM83::LD(ByteRegister& reg1, ByteRegister& reg2){ reg1.Set(reg2.Get()); }
void SM83::LD_A_A(){ LD(a, a); }
void SM83::LD_A_B(){ LD(a, b); }
void SM83::LD_A_C(){ LD(a, c); }
void SM83::LD_A_D(){ LD(a, d); }
void SM83::LD_A_E(){ LD(a, e); }
void SM83::LD_A_H(){ LD(a, h); }
void SM83::LD_A_L(){ LD(a, l); }
void SM83::LD_A_n16()
{ 
    a.Set(gb.ReadMem(GetWordFromPC()));
}

void SM83::LD_B_A(){ LD(b, a); }
void SM83::LD_B_B(){ LD(b, b); }
void SM83::LD_B_C(){ LD(b, c); }
void SM83::LD_B_D(){ LD(b, d); }
void SM83::LD_B_E(){ LD(b, e); }
void SM83::LD_B_H(){ LD(b, h); }
void SM83::LD_B_L(){ LD(b, l); }

void SM83::LD_C_A(){ LD(c, a); }
void SM83::LD_C_B(){ LD(c, b); }
void SM83::LD_C_C(){ LD(c, c); }
void SM83::LD_C_D(){ LD(c, d); }
void SM83::LD_C_E(){ LD(c, e); }
void SM83::LD_C_H(){ LD(c, h); }
void SM83::LD_C_L(){ LD(c, l); }

void SM83::LD_D_A(){ LD(d, a); }
void SM83::LD_D_B(){ LD(d, b); }
void SM83::LD_D_C(){ LD(d, c); }
void SM83::LD_D_D(){ LD(d, d); }
void SM83::LD_D_E(){ LD(d, e); }
void SM83::LD_D_H(){ LD(d, h); }
void SM83::LD_D_L(){ LD(d, l); }

void SM83::LD_E_A(){ LD(e, a); }
void SM83::LD_E_B(){ LD(e, b); }
void SM83::LD_E_C(){ LD(e, c); }
void SM83::LD_E_D(){ LD(e, d); }
void SM83::LD_E_E(){ LD(e, e); }
void SM83::LD_E_H(){ LD(e, h); }
void SM83::LD_E_L(){ LD(e, l); }

void SM83::LD_H_A(){ LD(h, a); }
void SM83::LD_H_B(){ LD(h, b); }
void SM83::LD_H_C(){ LD(h, c); }
void SM83::LD_H_D(){ LD(h, d); }
void SM83::LD_H_E(){ LD(h, e); }
void SM83::LD_H_H(){ LD(h, h); }
void SM83::LD_H_L(){ LD(h, l); }

void SM83::LD_L_A(){ LD(l, a); }
void SM83::LD_L_B(){ LD(l, b); }
void SM83::LD_L_C(){ LD(l, c); }
void SM83::LD_L_D(){ LD(l, d); }
void SM83::LD_L_E(){ LD(l, e); }
void SM83::LD_L_H(){ LD(l, h); }
void SM83::LD_L_L(){ LD(l, l); }

void SM83::LD_n8(ByteRegister& reg){ reg.Set(GetByteFromPC()); }
void SM83::LD_A(){ LD_n8(a); }
void SM83::LD_B(){ LD_n8(b); }
void SM83::LD_C(){ LD_n8(c); }
void SM83::LD_D(){ LD_n8(d); }
void SM83::LD_E(){ LD_n8(e); }
void SM83::LD_H(){ LD_n8(h); }
void SM83::LD_L(){ LD_n8(l); }

void SM83::LD_BC(){ Set16(b, c, GetWordFromPC()); }
void SM83::LD_DE(){ Set16(d, e, GetWordFromPC()); }
void SM83::LD_HL(){ Set16(h, l, GetWordFromPC()); }

void SM83::LD_HL_addr_r8(ByteRegister& reg){ gb.WriteMem(HL(), reg.Get()); }
void SM83::LD_HL_addr_A(){ LD_HL_addr_r8(a); }
void SM83::LD_HL_addr_B(){ LD_HL_addr_r8(b); }
void SM83::LD_HL_addr_C(){ LD_HL_addr_r8(c); }
void SM83::LD_HL_addr_D(){ LD_HL_addr_r8(d); }
void SM83::LD_HL_addr_E(){ LD_HL_addr_r8(e); }
void SM83::LD_HL_addr_H(){ LD_HL_addr_r8(h); }
void SM83::LD_HL_addr_L(){ LD_HL_addr_r8(l); }

void SM83::LD_HL_addr_n8(){ gb.WriteMem(HL(), GetByteFromPC()); }

void SM83::LD_r8_HL_addr(ByteRegister& reg){ reg.Set(gb.ReadMem(HL())); }
void SM83::LD_A_HL_addr(){ LD_r8_HL_addr(a); }
void SM83::LD_B_HL_addr(){ LD_r8_HL_addr(b); }
void SM83::LD_C_HL_addr(){ LD_r8_HL_addr(c); }
void SM83::LD_D_HL_addr(){ LD_r8_HL_addr(d); }
void SM83::LD_E_HL_addr(){ LD_r8_HL_addr(e); }
void SM83::LD_H_HL_addr(){ LD_r8_HL_addr(h); }
void SM83::LD_L_HL_addr(){ LD_r8_HL_addr(l); }

void SM83::LD_BC_A(){ gb.WriteMem(BC(), a.Get()); }
void SM83::LD_DE_A(){ gb.WriteMem(DE(), a.Get()); }
void SM83::LD_HL_A(){ gb.WriteMem(HL(), a.Get()); }
void SM83::LD_addr_A(){ gb.WriteMem(GetWordFromPC(), a.Get()); }


void SM83::LD_A_BC(){ a.Set(gb.ReadMem(BC())); }
void SM83::LD_A_DE(){ a.Set(gb.ReadMem(DE())); }


void SM83::LDH_A_n16(){ a.Set(gb.ReadMem(0xFF00 + GetByteFromPC())); }
void SM83::LDH_A_c(){ a.Set(gb.ReadMem(0xFF00 + c.Get())); }
void SM83::LD_HLI_A(){ gb.WriteMem(HL(), a.Get()); Set16(h, l, HL() + 1); }
void SM83::LD_HLD_A(){ gb.WriteMem(HL(), a.Get()); Set16(h, l, HL() - 1); }
void SM83::LD_A_HLI(){ a.Set(gb.ReadMem(HL())); Set16(h, l, HL() + 1); }
void SM83::LD_A_HLD(){ a.Set(gb.ReadMem(HL())); Set16(h, l, HL() - 1); }
void SM83::LD_SP_n16(){ sp.Set(GetWordFromPC()); }
void SM83::LD_n16_SP()
{ 
    uint16_t addr = GetWordFromPC();
    gb.WriteMem(addr, sp.Get() & 0xFF);
    gb.WriteMem(addr + 1, sp.Get() >> 8);
}

void SM83::LD_HL_SP_e8()
{ 
    int8_t byte = static_cast<int8_t>(GetByteFromPC());
    uint16_t result = static_cast<uint16_t>(sp.Get() + byte);
    Set16(h,l, result); 
}

void SM83::LD_SP_HL()
{
    sp.Set(HL());
}

void SM83::OR_A_r8(ByteRegister& reg)
{
    uint8_t val = a.Get() | reg.Get();
    a.Set(val);
    f.SetZero(val == 0);
    f.SetSubtract(false);
    f.SetHalfCarry(false);
    f.SetCarry(false);
}

void SM83::OR_A_A(){ OR_A_r8(a); }
void SM83::OR_A_B(){ OR_A_r8(b); }
void SM83::OR_A_C(){ OR_A_r8(c); }
void SM83::OR_A_D(){ OR_A_r8(d); }
void SM83::OR_A_E(){ OR_A_r8(e); }
void SM83::OR_A_H(){ OR_A_r8(h); }
void SM83::OR_A_L(){ OR_A_r8(l); }
void SM83::OR_A_HL(){ a.Set(gb.ReadMem(HL()) | a.Get()); }
void SM83::OR_A_n8(){ a.Set(a.Get() | GetByteFromPC()); }

void SM83::POP_AF()
{
    uint8_t lo = gb.ReadMem(sp.Get());
    sp.Increment();
    uint8_t hi = gb.ReadMem(sp.Get());
    sp.Increment();

    f.Set(lo & 0xF0);
    a.Set(hi);

}

void SM83::POP_BC()
{
    uint8_t lo = gb.ReadMem(sp.Get());
    sp.Increment();
    uint8_t hi = gb.ReadMem(sp.Get());
    sp.Increment();

    c.Set(lo);
    b.Set(hi);
}

void SM83::POP_DE()
{
    uint8_t lo = gb.ReadMem(sp.Get());
    sp.Increment();
    uint8_t hi = gb.ReadMem(sp.Get());
    sp.Increment();

    e.Set(lo);
    d.Set(hi);
}

void SM83::POP_HL()
{
    uint8_t lo = gb.ReadMem(sp.Get());
    sp.Increment();
    uint8_t hi = gb.ReadMem(sp.Get());
    sp.Increment();

    l.Set(lo);
    h.Set(hi);
}

void SM83::PUSH_AF()
{
    sp.Decrement();
    gb.WriteMem(sp.Get(), a.Get());
    sp.Decrement();
    gb.WriteMem(sp.Get(), f.Get());
}

void SM83::PUSH_BC()
{
    sp.Decrement();
    gb.WriteMem(sp.Get(), b.Get());
    sp.Decrement();
    gb.WriteMem(sp.Get(), c.Get());
}

void SM83::PUSH_DE()
{
    sp.Decrement();
    gb.WriteMem(sp.Get(), d.Get());
    sp.Decrement();
    gb.WriteMem(sp.Get(), e.Get());
}

void SM83::PUSH_HL()
{
    sp.Decrement();
    gb.WriteMem(sp.Get(), h.Get());
    sp.Decrement();
    gb.WriteMem(sp.Get(), l.Get());
}

void SM83::RES_r8(uint8_t bit, ByteRegister& reg)
{
    reg.Set(set_bit(reg.Get(),  bit));
}

void SM83::RES_HL(uint8_t bit)
{
    uint8_t val = gb.ReadMem(HL());
    gb.WriteMem(HL(), set_bit(val,  bit));
}

void SM83::RES_0_A(){ RES_r8(0, a); }
void SM83::RES_1_A(){ RES_r8(1, a); }
void SM83::RES_2_A(){ RES_r8(2, a); }
void SM83::RES_3_A(){ RES_r8(3, a); }
void SM83::RES_4_A(){ RES_r8(4, a); }
void SM83::RES_5_A(){ RES_r8(5, a); }
void SM83::RES_6_A(){ RES_r8(6, a); }
void SM83::RES_7_A(){ RES_r8(7, a); }

void SM83::RES_0_B(){ RES_r8(0, b); }
void SM83::RES_1_B(){ RES_r8(1, b); }
void SM83::RES_2_B(){ RES_r8(2, b); }
void SM83::RES_3_B(){ RES_r8(3, b); }
void SM83::RES_4_B(){ RES_r8(4, b); }
void SM83::RES_5_B(){ RES_r8(5, b); }
void SM83::RES_6_B(){ RES_r8(6, b); }
void SM83::RES_7_B(){ RES_r8(7, b); }

void SM83::RES_0_C(){ RES_r8(0, c); }
void SM83::RES_1_C(){ RES_r8(1, c); }
void SM83::RES_2_C(){ RES_r8(2, c); }
void SM83::RES_3_C(){ RES_r8(3, c); }
void SM83::RES_4_C(){ RES_r8(4, c); }
void SM83::RES_5_C(){ RES_r8(5, c); }
void SM83::RES_6_C(){ RES_r8(6, c); }
void SM83::RES_7_C(){ RES_r8(7, c); }

void SM83::RES_0_D(){ RES_r8(0, d); }
void SM83::RES_1_D(){ RES_r8(1, d); }
void SM83::RES_2_D(){ RES_r8(2, d); }
void SM83::RES_3_D(){ RES_r8(3, d); }
void SM83::RES_4_D(){ RES_r8(4, d); }
void SM83::RES_5_D(){ RES_r8(5, d); }
void SM83::RES_6_D(){ RES_r8(6, d); }
void SM83::RES_7_D(){ RES_r8(7, d); }


void SM83::RES_0_E(){ RES_r8(0, e); }
void SM83::RES_1_E(){ RES_r8(1, e); }
void SM83::RES_2_E(){ RES_r8(2, e); }
void SM83::RES_3_E(){ RES_r8(3, e); }
void SM83::RES_4_E(){ RES_r8(4, e); }
void SM83::RES_5_E(){ RES_r8(5, e); }
void SM83::RES_6_E(){ RES_r8(6, e); }
void SM83::RES_7_E(){ RES_r8(7, e); }

void SM83::RES_0_H(){ RES_r8(0, h); }
void SM83::RES_1_H(){ RES_r8(1, h); }
void SM83::RES_2_H(){ RES_r8(2, h); }
void SM83::RES_3_H(){ RES_r8(3, h); }
void SM83::RES_4_H(){ RES_r8(4, h); }
void SM83::RES_5_H(){ RES_r8(5, h); }
void SM83::RES_6_H(){ RES_r8(6, h); }
void SM83::RES_7_H(){ RES_r8(7, h); }


void SM83::RES_0_L(){ RES_r8(7, l); }
void SM83::RES_1_L(){ RES_r8(7, l); }
void SM83::RES_2_L(){ RES_r8(7, l); }
void SM83::RES_3_L(){ RES_r8(7, l); }
void SM83::RES_4_L(){ RES_r8(7, l); }
void SM83::RES_5_L(){ RES_r8(7, l); }
void SM83::RES_6_L(){ RES_r8(7, l); }
void SM83::RES_7_L(){ RES_r8(7, l); }

void SM83::RES_0_HL(){ RES_HL(0); }
void SM83::RES_1_HL(){ RES_HL(1); }
void SM83::RES_2_HL(){ RES_HL(2); }
void SM83::RES_3_HL(){ RES_HL(3); }
void SM83::RES_4_HL(){ RES_HL(4); }
void SM83::RES_5_HL(){ RES_HL(5); }
void SM83::RES_6_HL(){ RES_HL(6); }
void SM83::RES_7_HL(){ RES_HL(7); }

void SM83::RET()
{
    uint8_t lo = gb.ReadMem(sp.Get());
    sp.Increment();
    uint8_t hi = gb.ReadMem(sp.Get());
    sp.Increment();

    pc.Set(MakeWord(lo, hi));
}

void SM83::RET(Condition condition)
{
    if(ConditionMet(condition)){
        RET();
    }
}

void SM83::RET_Z(){ RET(Condition::Z); }
void SM83::RET_NZ(){ RET(Condition::NZ); }
void SM83::RET_C(){ RET(Condition::C); }
void SM83::RET_NC(){ RET(Condition::NC); }

void SM83::RETI()
{
    RET();
    EI();
}

void SM83::RL_r8(ByteRegister& reg)
{
    uint8_t val = reg.Get();


    uint8_t oldCarry = f.Carry() ? 1 : 0;


    bool newCarry = (val & 0x80) != 0;

    uint8_t result = (val << 1) | oldCarry;

    reg.Set(result);

    f.SetZero(result == 0);
    f.SetSubtract(false);
    f.SetHalfCarry(false);
    f.SetCarry(newCarry);
}

void SM83::RL_A(){ RL_r8(a); }
void SM83::RL_B(){ RL_r8(d); }
void SM83::RL_C(){ RL_r8(c); }
void SM83::RL_D(){ RL_r8(d); }
void SM83::RL_E(){ RL_r8(e); }
void SM83::RL_H(){ RL_r8(h); }
void SM83::RL_L(){ RL_r8(l); }

void SM83::RL_HL()
{
    uint8_t val = gb.ReadMem(HL());

    uint8_t oldCarry = f.Carry() ? 1 : 0;


    bool newCarry = (val & 0x80) != 0;

    uint8_t result = (val << 1) | oldCarry;

    gb.WriteMem(HL(), result);

    f.SetZero(result == 0);
    f.SetSubtract(false);
    f.SetHalfCarry(false);
    f.SetCarry(newCarry);
}

void SM83::RLA()
{
    uint8_t val = a.Get();

    uint8_t oldCarry = f.Carry() ? 1 : 0;


    bool newCarry = (val & 0x80) != 0;

    uint8_t result = (val << 1) | oldCarry;

    a.Set(result);

    f.SetZero(false);
    f.SetSubtract(false);
    f.SetHalfCarry(false);
    f.SetCarry(newCarry);
}

void SM83::RLC_r8(ByteRegister& reg)
{
    uint8_t val = reg.Get();

    bool newCarry = (val & 0x80) != 0;
    uint8_t result = (val << 1) | (newCarry ? 1 : 0);

    reg.Set(result);

    f.SetZero(result == 0);
    f.SetSubtract(false);
    f.SetHalfCarry(false);
    f.SetCarry(newCarry);
}

void SM83::RLC_A(){ RLC_r8(a); }
void SM83::RLC_B(){ RLC_r8(b); }
void SM83::RLC_C(){ RLC_r8(c); }
void SM83::RLC_D(){ RLC_r8(d); }
void SM83::RLC_E(){ RLC_r8(e); }
void SM83::RLC_H(){ RLC_r8(h); }
void SM83::RLC_L(){ RLC_r8(l); }
    
void SM83::RLC_HL()
{
    uint8_t val = gb.ReadMem(HL());

    bool newCarry = (val & 0x80) != 0;
    uint8_t result = (val << 1) | (newCarry ? 1 : 0);

    gb.WriteMem(HL(), result);

    f.SetZero(result == 0);
    f.SetSubtract(false);
    f.SetHalfCarry(false);
    f.SetCarry(newCarry);
}

void SM83::RLCA()
{
    uint8_t val = a.Get();

    bool newCarry = (val & 0x80) != 0;
    uint8_t result = (val << 1) | (newCarry ? 1 : 0);

    a.Set(result);

    f.SetZero(false);
    f.SetSubtract(false);
    f.SetHalfCarry(false);
    f.SetCarry(newCarry);
}

void SM83::RR_r8(ByteRegister& reg)
{
    uint8_t val = reg.Get();

    uint8_t oldCarry = f.Carry() ? 0x80 : 0x00;

    bool newCarry = (val & 0x01) != 0;

    uint8_t result = (val >> 1) | oldCarry;

    reg.Set(result);

    f.SetZero(result == 0);
    f.SetSubtract(false);
    f.SetHalfCarry(false);
    f.SetCarry(newCarry);
}

void SM83::RR_A(){ RR_r8(a); }
void SM83::RR_B(){ RR_r8(b); }
void SM83::RR_C(){ RR_r8(c); }
void SM83::RR_D(){ RR_r8(d); }
void SM83::RR_E(){ RR_r8(e); }
void SM83::RR_H(){ RR_r8(h); }
void SM83::RR_L(){ RR_r8(l); }

void SM83::RR_HL()
{
    uint8_t val = gb.ReadMem(HL());

    uint8_t oldCarry = f.Carry() ? 0x80 : 0x00;

    bool newCarry = (val & 0x01) != 0;

    uint8_t result = (val >> 1) | oldCarry;

    gb.WriteMem(HL(), result);

    f.SetZero(result == 0);
    f.SetSubtract(false);
    f.SetHalfCarry(false);
    f.SetCarry(newCarry);
}

void SM83::RRA()
{
    uint8_t val = a.Get();

    uint8_t oldCarry = f.Carry() ? 0x80 : 0x00;

    bool newCarry = (val & 0x01) != 0;

    uint8_t result = (val >> 1) | oldCarry;

    a.Set(result);

    f.SetZero(false);
    f.SetSubtract(false);
    f.SetHalfCarry(false);
    f.SetCarry(newCarry);
}

void SM83::RRC_r8(ByteRegister& reg)
{
    uint8_t val = reg.Get();

    bool newCarry = (val & 0x01) != 0;
    uint8_t result = (val >> 1) | (newCarry ? 0x80 : 0x00);

    reg.Set(result);

    f.SetZero(result == 0);
    f.SetSubtract(false);
    f.SetHalfCarry(false);
    f.SetCarry(newCarry);
}

void SM83::RRC_A(){ RRC_r8(a); }
void SM83::RRC_B(){ RRC_r8(a); }
void SM83::RRC_C(){ RRC_r8(a); }
void SM83::RRC_D(){ RRC_r8(a); }
void SM83::RRC_E(){ RRC_r8(a); }
void SM83::RRC_H(){ RRC_r8(a); }
void SM83::RRC_L(){ RRC_r8(a); }
void SM83::RRC_HL()
{ 
    uint8_t val = gb.ReadMem(HL());

    bool newCarry = (val & 0x01) != 0;
    uint8_t result = (val >> 1) | (newCarry ? 0x80 : 0x00);

    gb.WriteMem(HL(), result);

    f.SetZero(result == 0);
    f.SetSubtract(false);
    f.SetHalfCarry(false);
    f.SetCarry(newCarry);
}

void SM83::RRCA()
{
    uint8_t val = a.Get();

    bool newCarry = (val & 0x01) != 0;
    uint8_t result = (val >> 1) | (newCarry ? 0x80 : 0x00);

    a.Set(result);

    f.SetZero(false);
    f.SetSubtract(false);
    f.SetHalfCarry(false);
    f.SetCarry(newCarry);
}

void SM83::RST(const uint8_t offset)
{
    StackPush(pc.Get());
    pc.Set(offset);
}

void SM83::RST_00(){ RST(0x00); }
void SM83::RST_08(){ RST(0x08); }
void SM83::RST_10(){ RST(0x10); }
void SM83::RST_18(){ RST(0x18); }
void SM83::RST_20(){ RST(0x20); }
void SM83::RST_28(){ RST(0x28); }
void SM83::RST_30(){ RST(0x30); }
void SM83::RST_38(){ RST(0x38); }

void SM83::SBC_A_r8(ByteRegister& reg)
{
    uint8_t carry = f.Carry();
    uint8_t aVal = a.Get();

    int result_full = aVal - reg.Get() - carry;
    uint8_t result = static_cast<uint8_t>(result_full);

    f.SetZero(result == 0);
    f.SetSubtract(true);
    f.SetCarry(result_full < 0);
    f.SetHalfCarry(((aVal & 0xf) - (reg.Get() & 0xf) - carry) < 0);

    a.Set(result);
}

void SM83::SBC_A_A(){ SBC_A_r8(a); }
void SM83::SBC_A_B(){ SBC_A_r8(b); }
void SM83::SBC_A_C(){ SBC_A_r8(c); }
void SM83::SBC_A_D(){ SBC_A_r8(d); }
void SM83::SBC_A_E(){ SBC_A_r8(e); }
void SM83::SBC_A_H(){ SBC_A_r8(h); }
void SM83::SBC_A_L(){ SBC_A_r8(l); }

void SM83::SBC_A_HL()
{
    uint8_t carry = f.Carry();
    uint8_t aVal = a.Get();
    uint8_t val = gb.ReadMem(HL());

    int result_full = aVal - val - carry;
    uint8_t result = static_cast<uint8_t>(result_full);

    f.SetZero(result == 0);
    f.SetSubtract(true);
    f.SetCarry(result_full < 0);
    f.SetHalfCarry(((aVal & 0xf) - (val & 0xf) - carry) < 0);

    a.Set(result);
}

void SM83::SBC_A_n8()
{
    uint8_t carry = f.Carry();
    uint8_t aVal = a.Get();
    uint8_t val = GetByteFromPC();

    int result_full = aVal - val - carry;
    uint8_t result = static_cast<uint8_t>(result_full);

    f.SetZero(result == 0);
    f.SetSubtract(true);
    f.SetCarry(result_full < 0);
    f.SetHalfCarry(((aVal & 0xf) - (val & 0xf) - carry) < 0);

    a.Set(result);
}

void SM83::SCF()
{
    f.SetSubtract(false);
    f.SetHalfCarry(false);
    f.SetCarry(true);
}

void SM83::SET_r8(uint8_t bit, ByteRegister& reg)
{
    reg.Set(set_bit(reg.Get(), bit));
}

void SM83::SET_HL(uint8_t bit)
{
    uint8_t val = gb.ReadMem(HL());
    gb.WriteMem(HL(), set_bit(val, bit));
}

void SM83::SET_0_A(){ SET_r8(0, a); }
void SM83::SET_1_A(){ SET_r8(1, a); } // 0xCB 0x4F
void SM83::SET_2_A(){ SET_r8(2, a); } // 0xCB 0x57
void SM83::SET_3_A(){ SET_r8(3, a); } // 0xCB 0x5F
void SM83::SET_4_A(){ SET_r8(4, a); } // 0xCB 0x67
void SM83::SET_5_A(){ SET_r8(5, a); } // 0xCB 0x6F
void SM83::SET_6_A(){ SET_r8(6, a); } // 0xCB 0x77
void SM83::SET_7_A(){ SET_r8(7, a); } // 0xCB 0x7F

void SM83::SET_0_B(){ SET_r8(0, b); } // 0xCB 0x40
void SM83::SET_1_B(){ SET_r8(1, b); } // 0xCB 0x48
void SM83::SET_2_B(){ SET_r8(2, b); } // 0xCB 0x50
void SM83::SET_3_B(){ SET_r8(3, b); } // 0xCB 0x58
void SM83::SET_4_B(){ SET_r8(4, b); } // 0xCB 0x60
void SM83::SET_5_B(){ SET_r8(5, b); } // 0xCB 0x68
void SM83::SET_6_B(){ SET_r8(6, b); } // 0xCB 0x70
void SM83::SET_7_B(){ SET_r8(7, b); } // 0xCB 0x78

void SM83::SET_0_C(){ SET_r8(0, c); } // 0xCB 0x41
void SM83::SET_1_C(){ SET_r8(1, c); } // 0xCB 0x49
void SM83::SET_2_C(){ SET_r8(2, c); } // 0xCB 0x51
void SM83::SET_3_C(){ SET_r8(3, c); } // 0xCB 0x59
void SM83::SET_4_C(){ SET_r8(4, c); } // 0xCB 0x61
void SM83::SET_5_C(){ SET_r8(5, c); } // 0xCB 0x69
void SM83::SET_6_C(){ SET_r8(6, c); } // 0xCB 0x71
void SM83::SET_7_C(){ SET_r8(7, c); } // 0xCB 0x79

void SM83::SET_0_D(){ SET_r8(0, d); } // 0xCB 0x42
void SM83::SET_1_D(){ SET_r8(1, d); } // 0xCB 0x4A
void SM83::SET_2_D(){ SET_r8(2, d); } // 0xCB 0x52
void SM83::SET_3_D(){ SET_r8(3, d); } // 0xCB 0x5A
void SM83::SET_4_D(){ SET_r8(4, d); } // 0xCB 0x62
void SM83::SET_5_D(){ SET_r8(5, d); } // 0xCB 0x6A
void SM83::SET_6_D(){ SET_r8(6, d); } // 0xCB 0x72
void SM83::SET_7_D(){ SET_r8(7, d); } // 0xCB 0x7A


void SM83::SET_0_E(){ SET_r8(0, e); } // 0xCB 0x43
void SM83::SET_1_E(){ SET_r8(1, e); } // 0xCB 0x4B
void SM83::SET_2_E(){ SET_r8(2, e); } // 0xCB 0x53
void SM83::SET_3_E(){ SET_r8(3, e); } // 0xCB 0x5B
void SM83::SET_4_E(){ SET_r8(4, e); } // 0xCB 0x63
void SM83::SET_5_E(){ SET_r8(5, e); } // 0xCB 0x6B
void SM83::SET_6_E(){ SET_r8(6, e); } // 0xCB 0x73
void SM83::SET_7_E(){ SET_r8(7, e); } // 0xCB 0x7B

void SM83::SET_0_H(){ SET_r8(0, h); } // 0xCB 0x44
void SM83::SET_1_H(){ SET_r8(1, h); } // 0xCB 0x4C
void SM83::SET_2_H(){ SET_r8(2, h); } // 0xCB 0x54
void SM83::SET_3_H(){ SET_r8(3, h); } // 0xCB 0x5C
void SM83::SET_4_H(){ SET_r8(4, h); } // 0xCB 0x64
void SM83::SET_5_H(){ SET_r8(5, h); } // 0xCB 0x6C
void SM83::SET_6_H(){ SET_r8(6, h); } // 0xCB 0x74
void SM83::SET_7_H(){ SET_r8(7, h); } // 0xCB 0x7C


void SM83::SET_0_L(){ SET_r8(0, l); } // 0xCB 0x45
void SM83::SET_1_L(){ SET_r8(1, l); } // 0xCB 0x4D
void SM83::SET_2_L(){ SET_r8(2, l); } // 0xCB 0x55
void SM83::SET_3_L(){ SET_r8(3, l); } // 0xCB 0x5D
void SM83::SET_4_L(){ SET_r8(4, l); } // 0xCB 0x65
void SM83::SET_5_L(){ SET_r8(5, l); } // 0xCB 0x6D
void SM83::SET_6_L(){ SET_r8(6, l); } // 0xCB 0x75
void SM83::SET_7_L(){ SET_r8(7, l); } // 0xCB 0x7D

void SM83::SET_0_HL(){ SET_HL(0); } // 0xCB 0x46
void SM83::SET_1_HL(){ SET_HL(1); } // 0xCB 0x4E
void SM83::SET_2_HL(){ SET_HL(2); } // 0xCB 0x56
void SM83::SET_3_HL(){ SET_HL(3); } // 0xCB 0x5E
void SM83::SET_4_HL(){ SET_HL(4); } // 0xCB 0x66
void SM83::SET_5_HL(){ SET_HL(5); } // 0xCB 0x6E
void SM83::SET_6_HL(){ SET_HL(6); } // 0xCB 0x76
void SM83::SET_7_HL(){ SET_HL(7); } // 0xCB 0x7E


void SM83::SLA(ByteRegister& reg)
{
    uint8_t val = reg.Get();

    bool newCarry = (val & 0x80) != 0;
    uint8_t result = val << 1;

    reg.Set(result);

    f.SetZero(result == 0);
    f.SetSubtract(false);
    f.SetHalfCarry(false);
    f.SetCarry(newCarry);
}

void SM83::SLA_A(){ SLA(a); }
void SM83::SLA_B(){ SLA(b); }
void SM83::SLA_C(){ SLA(c); }
void SM83::SLA_D(){ SLA(d); }
void SM83::SLA_E(){ SLA(e); }
void SM83::SLA_H(){ SLA(h); }
void SM83::SLA_L(){ SLA(l); }

void SM83::SLA_HL()
{
    uint8_t val = gb.ReadMem(HL());

    bool newCarry = (val & 0x80) != 0;
    uint8_t result = val << 1;

    gb.WriteMem(HL(), result);

    f.SetZero(result == 0);
    f.SetSubtract(false);
    f.SetHalfCarry(false);
    f.SetCarry(newCarry);
}

void SM83::SRA_r8(ByteRegister& reg)
{
    uint8_t val = reg.Get();

    bool newCarry = (val & 0x01) != 0;
    uint8_t msb = val & 0x80;
    uint8_t result = (val >> 1) | msb;

    reg.Set(result);

    f.SetZero(result == 0);
    f.SetSubtract(false);
    f.SetHalfCarry(false);
    f.SetCarry(newCarry);
}

void SM83::SRA_A(){ SRA_r8(a); }
void SM83::SRA_B(){ SRA_r8(a); }
void SM83::SRA_C(){ SRA_r8(a); }
void SM83::SRA_D(){ SRA_r8(a); }
void SM83::SRA_E(){ SRA_r8(a); }
void SM83::SRA_H(){ SRA_r8(a); }
void SM83::SRA_L(){ SRA_r8(a); }

void SM83::SRA_HL()
{
    uint8_t val = gb.ReadMem(HL());

    bool newCarry = (val & 0x01) != 0;
    uint8_t msb = val & 0x80;
    uint8_t result = (val >> 1) | msb;

    gb.WriteMem(HL(), result);

    f.SetZero(result == 0);
    f.SetSubtract(false);
    f.SetHalfCarry(false);
    f.SetCarry(newCarry);
}

void SM83::SRL_r8(ByteRegister& reg)
{
    uint8_t value = reg.Get();
    f.SetSubtract(false);            
    f.SetHalfCarry(false);               
    f.SetCarry(value & 0x01);          

    value >>= 1;                     
    reg.Set(value);

    f.SetZero(value == 0);
}

void SM83::SRL_A(){ SRL_r8(a); }
void SM83::SRL_B(){ SRL_r8(b); }
void SM83::SRL_C(){ SRL_r8(c); }
void SM83::SRL_D(){ SRL_r8(d); }
void SM83::SRL_E(){ SRL_r8(e); }
void SM83::SRL_H(){ SRL_r8(h); }
void SM83::SRL_L(){ SRL_r8(l); }

void SM83::SRL_HL()
{
    uint8_t value = gb.ReadMem(HL());
    f.SetSubtract(false);            
    f.SetHalfCarry(false);               
    f.SetCarry(value & 0x01);          

    value >>= 1;                     
    gb.WriteMem(HL(), value);

    f.SetZero(value == 0);
}

void SM83::STOP()
{

}

void SM83::SUB_A_r8(ByteRegister& reg)
{
    uint8_t A = a.Get();
    uint8_t val = reg.Get();
    uint8_t result = A - val;

    f.SetZero(result == 0);
    f.SetSubtract(true);                           
    f.SetHalfCarry((A & 0x0F) < (val & 0x0F));
    f.SetCarry(A < val);                      

    a.Set(result);
}

void SM83::SUB_A_A(){ SUB_A_r8(a); }
void SM83::SUB_A_B(){ SUB_A_r8(b); }
void SM83::SUB_A_C(){ SUB_A_r8(c); }
void SM83::SUB_A_D(){ SUB_A_r8(d); }
void SM83::SUB_A_E(){ SUB_A_r8(e); }
void SM83::SUB_A_H(){ SUB_A_r8(h); }
void SM83::SUB_A_L(){ SUB_A_r8(l); }

void SM83::SUB_A_HL()
{
    uint8_t A = a.Get();
    uint8_t val = gb.ReadMem(HL());
    uint8_t result = A - val;

    f.SetZero(result == 0);
    f.SetSubtract(true);                           
    f.SetHalfCarry((A & 0x0F) < (val & 0x0F));
    f.SetCarry(A < val);                      

    a.Set(result);
}

void SM83::SUB_A()
{
    uint8_t A = a.Get();
    uint8_t val = GetByteFromPC();
    uint8_t result = A - val;

    f.SetZero(result == 0);
    f.SetSubtract(true);                           
    f.SetHalfCarry((A & 0x0F) < (val & 0x0F));
    f.SetCarry(A < val);                      

    a.Set(result);
}

void SM83::SWAP_r8(ByteRegister& reg)
{
    uint8_t value = reg.Get();
    value = (value << 4) | (value >> 4); 
    reg.Set(value);

    f.SetZero(value == 0);
    f.SetSubtract(false);   
    f.SetHalfCarry(false);
    f.SetCarry(false);       
}

void SM83::SWAP_A(){ SWAP_r8(a); }
void SM83::SWAP_B(){ SWAP_r8(b); }
void SM83::SWAP_C(){ SWAP_r8(c); }
void SM83::SWAP_D(){ SWAP_r8(d); }
void SM83::SWAP_E(){ SWAP_r8(e); }
void SM83::SWAP_H(){ SWAP_r8(h); }
void SM83::SWAP_L(){ SWAP_r8(l); }

void SM83::SWAP_HL()
{
    uint8_t value = gb.ReadMem(HL());
    value = (value << 4) | (value >> 4); 
    gb.WriteMem(HL(), value);

    f.SetZero(value == 0);
    f.SetSubtract(false);   
    f.SetHalfCarry(false);
    f.SetCarry(false); 
}

void SM83::XOR_A_r8(ByteRegister& reg)
{
    uint8_t result = a.Get() ^ reg.Get();
    a.Set(result);

    f.SetZero(result == 0);
    f.SetSubtract(false);      
    f.SetHalfCarry(false);       
    f.SetCarry(false);        
}

void SM83::XOR_A_A(){ XOR_A_r8(a); }
void SM83::XOR_A_B(){ XOR_A_r8(a); }
void SM83::XOR_A_C(){ XOR_A_r8(a); }
void SM83::XOR_A_D(){ XOR_A_r8(a); }
void SM83::XOR_A_E(){ XOR_A_r8(a); }
void SM83::XOR_A_H(){ XOR_A_r8(a); }
void SM83::XOR_A_L(){ XOR_A_r8(a); }

void SM83::XOR_A_HL()
{
    uint8_t result = a.Get() ^ gb.ReadMem(HL());
    a.Set(result);

    f.SetZero(result == 0);
    f.SetSubtract(false);      
    f.SetHalfCarry(false);       
    f.SetCarry(false);    
}

void SM83::XOR_A()
{
    uint8_t result = a.Get() ^ GetByteFromPC();
    a.Set(result);

    f.SetZero(result == 0);
    f.SetSubtract(false);      
    f.SetHalfCarry(false);       
    f.SetCarry(false); 
}