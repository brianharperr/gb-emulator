#pragma once

#include <cstdint>
#include "register.h"

class Gameboy;

enum class Condition {
    NZ,
    Z,
    NC,
    C
};

class SM83 {
public:
    SM83(Gameboy& gb);
    uint8_t Tick();
    bool IsHalted() { return halted; };
    uint8_t GetOpcode();
private:
    //Registers
    ByteRegister a, b, c, d, e, h, l;
    FlagRegister f;
    WordRegister pc, sp;

    bool halted;
    bool branched;
    bool interrupts_enabled;

    uint16_t AF() { return (a.Get() << 8) | f.Get(); }
    uint16_t BC() { return (b.Get() << 8) | c.Get(); }
    uint16_t DE() { return (d.Get() << 8) | e.Get(); }
    uint16_t HL() { return (h.Get() << 8) | l.Get(); }

    void SetAF(uint16_t val) { a.Set((val >> 8) & 0xFF); f.Set(val & 0xF0); }
    void SetBC(uint16_t val) { b.Set((val >> 8) & 0xFF); c.Set(val & 0xFF); }
    void SetDE(uint16_t val) { d.Set((val >> 8) & 0xFF); e.Set(val & 0xFF); }
    void SetHL(uint16_t val) { h.Set((val >> 8) & 0xFF); l.Set(val & 0xFF); }

    Gameboy& gb;

    inline void Set16(ByteRegister& hi, ByteRegister& lo, uint16_t value)
    {
        hi.Set((value >> 8) & 0xFF);
        lo.Set(value & 0xFF);
    }

    uint8_t GetByteFromPC();
    uint16_t GetWordFromPC();
    uint16_t MakeWord(uint8_t lo, uint8_t hi);
    bool ConditionMet(Condition condition);
    void StackPush(uint16_t data);
    //Instructions
    void NOP();

    /* ADC */
    void ADC(uint8_t data);
    void ADC_A_r8(ByteRegister& reg);

    void ADC_A_A(); // 0x8F
    void ADC_A_B(); // 0x88
    void ADC_A_C(); // 0x89
    void ADC_A_D(); // 0x8A
    void ADC_A_E(); // 0x8B
    void ADC_A_H(); // 0x8C
    void ADC_A_L(); // 0x8D
    void ADC_A_HL(); // 0x8E
    void ADC_A_n8(); // 0xCE

    /* ADD */
    void ADD_A(uint8_t data);
    void ADD_A_r8(ByteRegister& reg);
    void ADD_HL(uint16_t data);

    void ADD_A_A(); // 0x87
    void ADD_A_B(); // 0x80
    void ADD_A_C(); // 0x81
    void ADD_A_D(); // 0x82
    void ADD_A_E(); // 0x83
    void ADD_A_H(); // 0x84
    void ADD_A_L(); // 0x85
    void ADD_A_HL(); // 0x86
    void ADD_A_n8(); // 0xC6
    void ADD_HL_BC(); // 0x09
    void ADD_HL_DE(); // 0x19
    void ADD_HL_HL(); // 0x29
    void ADD_HL_SP(); // 0x39
    void ADD_SP_e8(); // 0xE8

    /* AND */
    void AND_A_r8(ByteRegister& reg);
    void AND_A_A();
    void AND_A_B();
    void AND_A_C();
    void AND_A_D();
    void AND_A_E();
    void AND_A_H();
    void AND_A_L();
    void AND_A_HL();
    void AND_A();

    /* BIT */
    void BIT_r8(uint8_t bit, ByteRegister& reg);
    void BIT_HL(uint8_t bit, uint8_t value);

    void BIT_0_A(); // 0xCB 0x47
    void BIT_1_A(); // 0xCB 0x4F
    void BIT_2_A(); // 0xCB 0x57
    void BIT_3_A(); // 0xCB 0x5F
    void BIT_4_A(); // 0xCB 0x67
    void BIT_5_A(); // 0xCB 0x6F
    void BIT_6_A(); // 0xCB 0x77
    void BIT_7_A(); // 0xCB 0x7F

    void BIT_0_B(); // 0xCB 0x40
    void BIT_1_B(); // 0xCB 0x48
    void BIT_2_B(); // 0xCB 0x50
    void BIT_3_B(); // 0xCB 0x58
    void BIT_4_B(); // 0xCB 0x60
    void BIT_5_B(); // 0xCB 0x68
    void BIT_6_B(); // 0xCB 0x70
    void BIT_7_B(); // 0xCB 0x78

    void BIT_0_C(); // 0xCB 0x41
    void BIT_1_C(); // 0xCB 0x49
    void BIT_2_C(); // 0xCB 0x51
    void BIT_3_C(); // 0xCB 0x59
    void BIT_4_C(); // 0xCB 0x61
    void BIT_5_C(); // 0xCB 0x69
    void BIT_6_C(); // 0xCB 0x71
    void BIT_7_C(); // 0xCB 0x79

    void BIT_0_D(); // 0xCB 0x42
    void BIT_1_D(); // 0xCB 0x4A
    void BIT_2_D(); // 0xCB 0x52
    void BIT_3_D(); // 0xCB 0x5A
    void BIT_4_D(); // 0xCB 0x62
    void BIT_5_D(); // 0xCB 0x6A
    void BIT_6_D(); // 0xCB 0x72
    void BIT_7_D(); // 0xCB 0x7A


    void BIT_0_E(); // 0xCB 0x43
    void BIT_1_E(); // 0xCB 0x4B
    void BIT_2_E(); // 0xCB 0x53
    void BIT_3_E(); // 0xCB 0x5B
    void BIT_4_E(); // 0xCB 0x63
    void BIT_5_E(); // 0xCB 0x6B
    void BIT_6_E(); // 0xCB 0x73
    void BIT_7_E(); // 0xCB 0x7B

    void BIT_0_H(); // 0xCB 0x44
    void BIT_1_H(); // 0xCB 0x4C
    void BIT_2_H(); // 0xCB 0x54
    void BIT_3_H(); // 0xCB 0x5C
    void BIT_4_H(); // 0xCB 0x64
    void BIT_5_H(); // 0xCB 0x6C
    void BIT_6_H(); // 0xCB 0x74
    void BIT_7_H(); // 0xCB 0x7C


    void BIT_0_L(); // 0xCB 0x45
    void BIT_1_L(); // 0xCB 0x4D
    void BIT_2_L(); // 0xCB 0x55
    void BIT_3_L(); // 0xCB 0x5D
    void BIT_4_L(); // 0xCB 0x65
    void BIT_5_L(); // 0xCB 0x6D
    void BIT_6_L(); // 0xCB 0x75
    void BIT_7_L(); // 0xCB 0x7D

    void BIT_0_HL(); // 0xCB 0x46
    void BIT_1_HL(); // 0xCB 0x4E
    void BIT_2_HL(); // 0xCB 0x56
    void BIT_3_HL(); // 0xCB 0x5E
    void BIT_4_HL(); // 0xCB 0x66
    void BIT_5_HL(); // 0xCB 0x6E
    void BIT_6_HL(); // 0xCB 0x76
    void BIT_7_HL(); // 0xCB 0x7E

    /* CALL */
    void CALL_CONDITION(Condition condition);
    void CALL();
    void CALL_NZ();
    void CALL_Z();
    void CALL_NC();
    void CALL_C();

    /* CCF */
    void CCF();

    /* CP */
    void CP_A(uint8_t value);
    void CP_A_A();
    void CP_A_B();
    void CP_A_C();
    void CP_A_D();
    void CP_A_E();
    void CP_A_H();
    void CP_A_L();
    void CP_A_n8();
    void CP_A_HL();

    /* CPL */
    void CPL();

    /* DAA */
    void DAA();

    /* DEC */
    void DEC(ByteRegister& reg);
    void DEC_A();
    void DEC_B();
    void DEC_C();
    void DEC_D();
    void DEC_E();
    void DEC_H();
    void DEC_L();
    void DEC_HL();
    void DEC_BC();
    void DEC_DE();
    void DEC_SP();
    void DEC_HL_addr();

    /* DI */
    void DI();

    /* EI */
    void EI();

    /* HALT */
    void HALT();

    /* INC */
    void INC(ByteRegister& reg);
    void INC_A();
    void INC_B();
    void INC_C();
    void INC_D();
    void INC_E();
    void INC_H();
    void INC_L();
    void INC_HL();
    void INC_BC();
    void INC_DE();
    void INC_SP();
    void INC_HL_addr();

    /* JP */
    void JP_CONDITION(Condition condition);
    void JP();
    void JP_NZ();
    void JP_Z();
    void JP_NC();
    void JP_C();
    void JP_HL();

    /* JR */
    void JR_CONDITION(Condition condition);
    void JR();
    void JR_NZ();
    void JR_Z();
    void JR_NC();
    void JR_C();

    /* LD */
    void LD(ByteRegister& reg1, ByteRegister& reg2);
    void LD_A_A();
    void LD_A_B();
    void LD_A_C();
    void LD_A_D();
    void LD_A_E();
    void LD_A_H();
    void LD_A_L();
    void LD_A_n16();

    void LD_B_A();
    void LD_B_B();
    void LD_B_C();
    void LD_B_D();
    void LD_B_E();
    void LD_B_H();
    void LD_B_L();

    void LD_C_A();
    void LD_C_B();
    void LD_C_C();
    void LD_C_D();
    void LD_C_E();
    void LD_C_H();
    void LD_C_L();

    void LD_D_A();
    void LD_D_B();
    void LD_D_C();
    void LD_D_D();
    void LD_D_E();
    void LD_D_H();
    void LD_D_L();

    void LD_E_A();
    void LD_E_B();
    void LD_E_C();
    void LD_E_D();
    void LD_E_E();
    void LD_E_H();
    void LD_E_L();

    void LD_H_A();
    void LD_H_B();
    void LD_H_C();
    void LD_H_D();
    void LD_H_E();
    void LD_H_H();
    void LD_H_L();

    void LD_L_A();
    void LD_L_B();
    void LD_L_C();
    void LD_L_D();
    void LD_L_E();
    void LD_L_H();
    void LD_L_L();

    void LD_n8(ByteRegister& reg);
    void LD_A();
    void LD_B();
    void LD_C();
    void LD_D();
    void LD_E();
    void LD_H();
    void LD_L();

    void LD_A_BC();
    void LD_A_DE();

    void LD_BC();
    void LD_DE();
    void LD_HL();
    void LD_HL_addr_r8(ByteRegister& reg);
    void LD_HL_addr_A();
    void LD_HL_addr_B();
    void LD_HL_addr_C();
    void LD_HL_addr_D();
    void LD_HL_addr_E();
    void LD_HL_addr_H();
    void LD_HL_addr_L();

    void LD_HL_addr_n8();

    void LD_r8_HL_addr(ByteRegister& reg);
    void LD_A_HL_addr();
    void LD_B_HL_addr();
    void LD_C_HL_addr();
    void LD_D_HL_addr();
    void LD_E_HL_addr();
    void LD_H_HL_addr();
    void LD_L_HL_addr();

    void LD_BC_A();
    void LD_DE_A();
    void LD_HL_A();
    void LD_addr_A();

    void LDH_A_n16();
    void LDH_A_c();
    void LD_HLI_A();
    void LD_HLD_A();
    void LD_A_HLI();
    void LD_A_HLD();
    void LD_SP_n16();
    void LD_n16_SP();
    void LD_HL_SP_e8();
    void LD_SP_HL();

    /* OR */
    void OR_A_r8(ByteRegister& reg);
    void OR_A_A();
    void OR_A_B();
    void OR_A_C();
    void OR_A_D();
    void OR_A_E();
    void OR_A_H();
    void OR_A_L();
    void OR_A_HL();
    void OR_A_n8();

    /* POP */
    void POP_AF();
    void POP_BC();
    void POP_DE();
    void POP_HL();

    /* PUSH */
    void PUSH_AF();
    void PUSH_BC();
    void PUSH_DE();
    void PUSH_HL();

    /* RES */
    void RES_r8(uint8_t bit, ByteRegister& reg);
    void RES_HL(uint8_t bit);

    void RES_0_A(); // 0xCB 0x47
    void RES_1_A(); // 0xCB 0x4F
    void RES_2_A(); // 0xCB 0x57
    void RES_3_A(); // 0xCB 0x5F
    void RES_4_A(); // 0xCB 0x67
    void RES_5_A(); // 0xCB 0x6F
    void RES_6_A(); // 0xCB 0x77
    void RES_7_A(); // 0xCB 0x7F

    void RES_0_B(); // 0xCB 0x40
    void RES_1_B(); // 0xCB 0x48
    void RES_2_B(); // 0xCB 0x50
    void RES_3_B(); // 0xCB 0x58
    void RES_4_B(); // 0xCB 0x60
    void RES_5_B(); // 0xCB 0x68
    void RES_6_B(); // 0xCB 0x70
    void RES_7_B(); // 0xCB 0x78

    void RES_0_C(); // 0xCB 0x41
    void RES_1_C(); // 0xCB 0x49
    void RES_2_C(); // 0xCB 0x51
    void RES_3_C(); // 0xCB 0x59
    void RES_4_C(); // 0xCB 0x61
    void RES_5_C(); // 0xCB 0x69
    void RES_6_C(); // 0xCB 0x71
    void RES_7_C(); // 0xCB 0x79

    void RES_0_D(); // 0xCB 0x42
    void RES_1_D(); // 0xCB 0x4A
    void RES_2_D(); // 0xCB 0x52
    void RES_3_D(); // 0xCB 0x5A
    void RES_4_D(); // 0xCB 0x62
    void RES_5_D(); // 0xCB 0x6A
    void RES_6_D(); // 0xCB 0x72
    void RES_7_D(); // 0xCB 0x7A


    void RES_0_E(); // 0xCB 0x43
    void RES_1_E(); // 0xCB 0x4B
    void RES_2_E(); // 0xCB 0x53
    void RES_3_E(); // 0xCB 0x5B
    void RES_4_E(); // 0xCB 0x63
    void RES_5_E(); // 0xCB 0x6B
    void RES_6_E(); // 0xCB 0x73
    void RES_7_E(); // 0xCB 0x7B

    void RES_0_H(); // 0xCB 0x44
    void RES_1_H(); // 0xCB 0x4C
    void RES_2_H(); // 0xCB 0x54
    void RES_3_H(); // 0xCB 0x5C
    void RES_4_H(); // 0xCB 0x64
    void RES_5_H(); // 0xCB 0x6C
    void RES_6_H(); // 0xCB 0x74
    void RES_7_H(); // 0xCB 0x7C


    void RES_0_L(); // 0xCB 0x45
    void RES_1_L(); // 0xCB 0x4D
    void RES_2_L(); // 0xCB 0x55
    void RES_3_L(); // 0xCB 0x5D
    void RES_4_L(); // 0xCB 0x65
    void RES_5_L(); // 0xCB 0x6D
    void RES_6_L(); // 0xCB 0x75
    void RES_7_L(); // 0xCB 0x7D

    void RES_0_HL(); // 0xCB 0x46
    void RES_1_HL(); // 0xCB 0x4E
    void RES_2_HL(); // 0xCB 0x56
    void RES_3_HL(); // 0xCB 0x5E
    void RES_4_HL(); // 0xCB 0x66
    void RES_5_HL(); // 0xCB 0x6E
    void RES_6_HL(); // 0xCB 0x76
    void RES_7_HL(); // 0xCB 0x7E

    /* RET */
    void RET();
    void RET(Condition condition);
    void RET_Z();
    void RET_NZ();
    void RET_C();
    void RET_NC();

    /* RETI */
    void RETI();

    /* RL */
    void RL_r8(ByteRegister& reg);
    void RL_A();
    void RL_B();
    void RL_C();
    void RL_D();
    void RL_E();
    void RL_H();
    void RL_L();
    void RL_HL();

    /* RLA */
    void RLA();

    /* RLC */
    void RLC_r8(ByteRegister& reg);
    void RLC_A();
    void RLC_B();
    void RLC_C();
    void RLC_D();
    void RLC_E();
    void RLC_H();
    void RLC_L();
    void RLC_HL();

    /* RLCA */
    void RLCA();

    /* RR */
    void RR_r8(ByteRegister& reg);
    void RR_A();
    void RR_B();
    void RR_C();
    void RR_D();
    void RR_E();
    void RR_H();
    void RR_L();
    void RR_HL();


    /* RRA */
    void RRA();

    /* RRC */
    void RRC_r8(ByteRegister& reg);
    void RRC_A();
    void RRC_B();
    void RRC_C();
    void RRC_D();
    void RRC_E();
    void RRC_H();
    void RRC_L();
    void RRC_HL();

    /* RRCA */
    void RRCA();

    /* RST */
    void RST(const uint8_t offset);
    void RST_00();
    void RST_08();
    void RST_10();
    void RST_18();
    void RST_20();
    void RST_28();
    void RST_30();
    void RST_38();

    /* SBC */
    void SBC_A_r8(ByteRegister& reg);
    void SBC_A_A();
    void SBC_A_B();
    void SBC_A_C();
    void SBC_A_D();
    void SBC_A_E();
    void SBC_A_H();
    void SBC_A_L();
    void SBC_A_HL();
    void SBC_A_n8();

    /* SCF */
    void SCF();

    /* SET */
    void SET_r8(uint8_t bit, ByteRegister& reg);
    void SET_HL(uint8_t bit);

    void SET_0_A(); // 0xCB 0x47
    void SET_1_A(); // 0xCB 0x4F
    void SET_2_A(); // 0xCB 0x57
    void SET_3_A(); // 0xCB 0x5F
    void SET_4_A(); // 0xCB 0x67
    void SET_5_A(); // 0xCB 0x6F
    void SET_6_A(); // 0xCB 0x77
    void SET_7_A(); // 0xCB 0x7F

    void SET_0_B(); // 0xCB 0x40
    void SET_1_B(); // 0xCB 0x48
    void SET_2_B(); // 0xCB 0x50
    void SET_3_B(); // 0xCB 0x58
    void SET_4_B(); // 0xCB 0x60
    void SET_5_B(); // 0xCB 0x68
    void SET_6_B(); // 0xCB 0x70
    void SET_7_B(); // 0xCB 0x78

    void SET_0_C(); // 0xCB 0x41
    void SET_1_C(); // 0xCB 0x49
    void SET_2_C(); // 0xCB 0x51
    void SET_3_C(); // 0xCB 0x59
    void SET_4_C(); // 0xCB 0x61
    void SET_5_C(); // 0xCB 0x69
    void SET_6_C(); // 0xCB 0x71
    void SET_7_C(); // 0xCB 0x79

    void SET_0_D(); // 0xCB 0x42
    void SET_1_D(); // 0xCB 0x4A
    void SET_2_D(); // 0xCB 0x52
    void SET_3_D(); // 0xCB 0x5A
    void SET_4_D(); // 0xCB 0x62
    void SET_5_D(); // 0xCB 0x6A
    void SET_6_D(); // 0xCB 0x72
    void SET_7_D(); // 0xCB 0x7A


    void SET_0_E(); // 0xCB 0x43
    void SET_1_E(); // 0xCB 0x4B
    void SET_2_E(); // 0xCB 0x53
    void SET_3_E(); // 0xCB 0x5B
    void SET_4_E(); // 0xCB 0x63
    void SET_5_E(); // 0xCB 0x6B
    void SET_6_E(); // 0xCB 0x73
    void SET_7_E(); // 0xCB 0x7B

    void SET_0_H(); // 0xCB 0x44
    void SET_1_H(); // 0xCB 0x4C
    void SET_2_H(); // 0xCB 0x54
    void SET_3_H(); // 0xCB 0x5C
    void SET_4_H(); // 0xCB 0x64
    void SET_5_H(); // 0xCB 0x6C
    void SET_6_H(); // 0xCB 0x74
    void SET_7_H(); // 0xCB 0x7C


    void SET_0_L(); // 0xCB 0x45
    void SET_1_L(); // 0xCB 0x4D
    void SET_2_L(); // 0xCB 0x55
    void SET_3_L(); // 0xCB 0x5D
    void SET_4_L(); // 0xCB 0x65
    void SET_5_L(); // 0xCB 0x6D
    void SET_6_L(); // 0xCB 0x75
    void SET_7_L(); // 0xCB 0x7D

    void SET_0_HL(); // 0xCB 0x46
    void SET_1_HL(); // 0xCB 0x4E
    void SET_2_HL(); // 0xCB 0x56
    void SET_3_HL(); // 0xCB 0x5E
    void SET_4_HL(); // 0xCB 0x66
    void SET_5_HL(); // 0xCB 0x6E
    void SET_6_HL(); // 0xCB 0x76
    void SET_7_HL(); // 0xCB 0x7E

    /* SLA */
    void SLA(ByteRegister& reg);
    void SLA_A();
    void SLA_B();
    void SLA_C();
    void SLA_D();
    void SLA_E();
    void SLA_H();
    void SLA_L();
    void SLA_HL();

    /* SRA */
    void SRA_r8(ByteRegister& reg);
    void SRA_A();
    void SRA_B();
    void SRA_C();
    void SRA_D();
    void SRA_E();
    void SRA_H();
    void SRA_L();
    void SRA_HL();

    /* SRL */
    void SRL_r8(ByteRegister& reg);
    void SRL_A();
    void SRL_B();
    void SRL_C();
    void SRL_D();
    void SRL_E();
    void SRL_H();
    void SRL_L();
    void SRL_HL();

    void STOP();

    /* SUB */
    void SUB_A_r8(ByteRegister& reg);
    void SUB_A_A();
    void SUB_A_B();
    void SUB_A_C();
    void SUB_A_D();
    void SUB_A_E();
    void SUB_A_H();
    void SUB_A_L();
    void SUB_A_HL();
    void SUB_A();

    /* SWAP */
    void SWAP_r8(ByteRegister& reg);
    void SWAP_A();
    void SWAP_B();
    void SWAP_C();
    void SWAP_D();
    void SWAP_E();
    void SWAP_H();
    void SWAP_L();
    void SWAP_HL();

    /* XOR */
    void XOR_A_r8(ByteRegister& reg);
    void XOR_A_A();
    void XOR_A_B();
    void XOR_A_C();
    void XOR_A_D();
    void XOR_A_E();
    void XOR_A_H();
    void XOR_A_L();
    void XOR_A_HL();
    void XOR_A();

    struct Opcode {
        void (SM83::*handler)();
        uint8_t cycles;
        uint8_t length;
    };

    static constexpr Opcode opcodeTable[256] = {
/* 00 */ { &SM83::NOP, 4, 1 },
/* 01 */ { &SM83::LD_BC, 12, 3 },
/* 02 */ { &SM83::LD_BC_A, 8, 1 },
/* 03 */ { &SM83::INC_BC, 8, 1 },
/* 04 */ { &SM83::INC_B, 4, 1 },
/* 05 */ { &SM83::DEC_B, 4, 1 },
/* 06 */ { &SM83::LD_B, 8, 2 },
/* 07 */ { &SM83::RLCA, 4, 1 },
/* 08 */ { &SM83::LD_n16_SP, 20, 3 },
/* 09 */ { &SM83::ADD_HL_BC, 8, 1 },
/* 0A */ { &SM83::LD_A_BC, 8, 1 },
/* 0B */ { &SM83::DEC_BC, 8, 1 },
/* 0C */ { &SM83::INC_C, 4, 1 },
/* 0D */ { &SM83::DEC_C, 4, 1 },
/* 0E */ { &SM83::LD_C, 8, 2 },
/* 0F */ { &SM83::RRCA, 4, 1 },

/* 10 */ { &SM83::STOP, 4, 2 },
/* 11 */ { &SM83::LD_DE, 12, 3 },
/* 12 */ { &SM83::LD_DE_A, 8, 1 },
/* 13 */ { &SM83::INC_DE, 8, 1 },
/* 14 */ { &SM83::INC_D, 4, 1 },
/* 15 */ { &SM83::DEC_D, 4, 1 },
/* 16 */ { &SM83::LD_D, 8, 2 },
/* 17 */ { &SM83::RLA, 4, 1 },
/* 18 */ { &SM83::JR, 12, 2 },
/* 19 */ { &SM83::ADD_HL_DE, 8, 1 },
/* 1A */ { &SM83::LD_A_DE, 8, 1 },
/* 1B */ { &SM83::DEC_DE, 8, 1 },
/* 1C */ { &SM83::INC_E, 4, 1 },
/* 1D */ { &SM83::DEC_E, 4, 1 },
/* 1E */ { &SM83::LD_E, 8, 2 },
/* 1F */ { &SM83::RRA, 4, 1 },

/* 20 */ { &SM83::JR_NZ, 12, 2 },
/* 21 */ { &SM83::LD_HL, 12, 3 },
/* 22 */ { &SM83::LD_HLI_A, 8, 1 },
/* 23 */ { &SM83::INC_HL, 8, 1 },
/* 24 */ { &SM83::INC_H, 4, 1 },
/* 25 */ { &SM83::DEC_H, 4, 1 },
/* 26 */ { &SM83::LD_H, 8, 2 },
/* 27 */ { &SM83::DAA, 4, 1 },
/* 28 */ { &SM83::JR_Z, 12, 2 },
/* 29 */ { &SM83::ADD_HL_HL, 8, 1 },
/* 2A */ { &SM83::LD_A_HLI, 8, 1 },
/* 2B */ { &SM83::DEC_HL, 8, 1 },
/* 2C */ { &SM83::INC_L, 4, 1 },
/* 2D */ { &SM83::DEC_L, 4, 1 },
/* 2E */ { &SM83::LD_L, 8, 2 },
/* 2F */ { &SM83::CPL, 4, 1 },

/* 30 */ { &SM83::JR_NC, 12, 2 },
/* 31 */ { &SM83::LD_SP_n16, 12, 3 },
/* 32 */ { &SM83::LD_HLD_A, 8, 1 },
/* 33 */ { &SM83::INC_SP, 8, 1 },
/* 34 */ { &SM83::INC_HL_addr, 12, 1 },
/* 35 */ { &SM83::DEC_HL_addr, 12, 1 },
/* 36 */ { &SM83::LD_HL_addr_n8, 12, 2 },
/* 37 */ { &SM83::SCF, 4, 1 },
/* 38 */ { &SM83::JR_C, 12, 2 },
/* 39 */ { &SM83::ADD_HL_SP, 8, 1 },
/* 3A */ { &SM83::LD_A_HLD, 8, 1 },
/* 3B */ { &SM83::DEC_SP, 8, 1 },
/* 3C */ { &SM83::INC_A, 4, 1 },
/* 3D */ { &SM83::DEC_A, 4, 1 },
/* 3E */ { &SM83::LD_A, 8, 2 },
/* 3F */ { &SM83::CCF, 4, 1 },

/* 40 */ { &SM83::LD_B_B, 4, 1},
/* 41 */ { &SM83::LD_B_C, 4, 1},
/* 42 */ { &SM83::LD_B_D, 4, 1},
/* 43 */ { &SM83::LD_B_E, 4, 1},
/* 44 */ { &SM83::LD_B_H, 4, 1},
/* 45 */ { &SM83::LD_B_L, 4, 1},
/* 46 */ { &SM83::LD_B_HL_addr, 8, 1},
/* 47 */ { &SM83::LD_B_A, 4, 1},
/* 48 */ { &SM83::LD_C_B, 4, 1},
/* 49 */ { &SM83::LD_C_C, 4, 1},
/* 4A */ { &SM83::LD_C_D, 4, 1},
/* 4B */ { &SM83::LD_C_E, 4, 1},
/* 4C */ { &SM83::LD_C_H, 4, 1},
/* 4D */ { &SM83::LD_C_L, 4, 1},
/* 4E */ { &SM83::LD_C_HL_addr, 8, 1},
/* 4F */ { &SM83::LD_C_A, 4, 1},

/* 50 */ { &SM83::LD_D_B, 4, 1},
/* 51 */ { &SM83::LD_D_C, 4, 1},
/* 52 */ { &SM83::LD_D_D, 4, 1},
/* 53 */ { &SM83::LD_D_E, 4, 1},
/* 54 */ { &SM83::LD_D_H, 4, 1},
/* 55 */ { &SM83::LD_D_L, 4, 1},
/* 56 */ { &SM83::LD_D_HL_addr, 8, 1},
/* 57 */ { &SM83::LD_D_A, 4, 1},
/* 58 */ { &SM83::LD_E_B, 4, 1},
/* 59 */ { &SM83::LD_E_C, 4, 1},
/* 5A */ { &SM83::LD_E_D, 4, 1},
/* 5B */ { &SM83::LD_E_E, 4, 1},
/* 5C */ { &SM83::LD_E_H, 4, 1},
/* 5D */ { &SM83::LD_E_L, 4, 1},
/* 5E */ { &SM83::LD_E_HL_addr, 8, 1},
/* 5F */ { &SM83::LD_E_A, 4, 1},

/* 60 */ { &SM83::LD_H_B, 4, 1},
/* 61 */ { &SM83::LD_H_C, 4, 1},
/* 62 */ { &SM83::LD_H_D, 4, 1},
/* 63 */ { &SM83::LD_H_E, 4, 1},
/* 64 */ { &SM83::LD_H_H, 4, 1},
/* 65 */ { &SM83::LD_H_L, 4, 1},
/* 66 */ { &SM83::LD_H_HL_addr, 8, 1},
/* 67 */ { &SM83::LD_H_A, 4, 1},
/* 68 */ { &SM83::LD_L_B, 4, 1},
/* 69 */ { &SM83::LD_L_C, 4, 1},
/* 6A */ { &SM83::LD_L_D, 4, 1},
/* 6B */ { &SM83::LD_L_E, 4, 1},
/* 6C */ { &SM83::LD_L_H, 4, 1},
/* 6D */ { &SM83::LD_L_L, 4, 1},
/* 6E */ { &SM83::LD_L_HL_addr, 8, 1},
/* 6F */ { &SM83::LD_L_A, 4, 1},

/* 70 */ { &SM83::LD_HL_addr_B, 8, 1},
/* 71 */ { &SM83::LD_HL_addr_C, 8, 1},
/* 72 */ { &SM83::LD_HL_addr_D, 8, 1},
/* 73 */ { &SM83::LD_HL_addr_E, 8, 1},
/* 74 */ { &SM83::LD_HL_addr_H, 8, 1},
/* 75 */ { &SM83::LD_HL_addr_L, 8, 1},
/* 76 */ { &SM83::HALT, 4, 1},
/* 77 */ { &SM83::LD_HL_addr_A, 8, 1},
/* 78 */ { &SM83::LD_A_B, 4, 1},
/* 79 */ { &SM83::LD_A_C, 4, 1},
/* 7A */ { &SM83::LD_A_D, 4, 1},
/* 7B */ { &SM83::LD_A_E, 4, 1},
/* 7C */ { &SM83::LD_A_H, 4, 1},
/* 7D */ { &SM83::LD_A_L, 4, 1},
/* 7E */ { &SM83::LD_A_HL_addr, 8, 1},
/* 7F */ { &SM83::LD_A_A, 4, 1},

/* 80 */ { &SM83::ADD_A_B,4,1 },
/* 81 */ { &SM83::ADD_A_C,4,1 },
/* 82 */ { &SM83::ADD_A_D,4,1 },
/* 83 */ { &SM83::ADD_A_E,4,1 },
/* 84 */ { &SM83::ADD_A_H,4,1 },
/* 85 */ { &SM83::ADD_A_L,4,1 },
/* 86 */ { &SM83::ADD_A_HL,8,1 },
/* 87 */ { &SM83::ADD_A_A,4,1 },
/* 88 */ { &SM83::ADC_A_B,4,1 },
/* 89 */ { &SM83::ADC_A_C,4,1 },
/* 8A */ { &SM83::ADC_A_D,4,1 },
/* 8B */ { &SM83::ADC_A_E,4,1 },
/* 8C */ { &SM83::ADC_A_H,4,1 },
/* 8D */ { &SM83::ADC_A_L,4,1 },
/* 8E */ { &SM83::ADC_A_HL,8,1 },
/* 8F */ { &SM83::ADC_A_A,4,1 },

/* 90 */ { &SM83::SUB_A_B,4,1 },
/* 91 */ { &SM83::SUB_A_C,4,1 },
/* 92 */ { &SM83::SUB_A_D,4,1 },
/* 93 */ { &SM83::SUB_A_E,4,1 },
/* 94 */ { &SM83::SUB_A_H,4,1 },
/* 95 */ { &SM83::SUB_A_L,4,1 },
/* 96 */ { &SM83::SUB_A_HL,8,1 },
/* 97 */ { &SM83::SUB_A_A,4,1 },
/* 98 */ { &SM83::SBC_A_B,4,1 },
/* 99 */ { &SM83::SBC_A_C,4,1 },
/* 9A */ { &SM83::SBC_A_D,4,1 },
/* 9B */ { &SM83::SBC_A_E,4,1 },
/* 9C */ { &SM83::SBC_A_H,4,1 },
/* 9D */ { &SM83::SBC_A_L,4,1 },
/* 9E */ { &SM83::SBC_A_HL,8,1 },
/* 9F */ { &SM83::SBC_A_A,4,1 },

/* A0 */ { &SM83::AND_A_B, 4, 1},
/* A1 */ { &SM83::AND_A_C, 4, 1},
/* A2 */ { &SM83::AND_A_D, 4, 1},
/* A3 */ { &SM83::AND_A_E, 4, 1},
/* A4 */ { &SM83::AND_A_H, 4, 1},
/* A5 */ { &SM83::AND_A_L, 4, 1},
/* A6 */ { &SM83::AND_A_HL, 8, 1},
/* A7 */ { &SM83::AND_A_A, 4, 1},
/* A8 */ { &SM83::ADC_A_B, 4, 1},
/* A9 */ { &SM83::ADC_A_C, 4, 1},
/* AA */ { &SM83::ADC_A_D, 4, 1},
/* AB */ { &SM83::ADC_A_E, 4, 1},
/* AC */ { &SM83::ADC_A_H, 4, 1},
/* AD */ { &SM83::ADC_A_L, 4, 1},
/* AE */ { &SM83::ADC_A_HL, 8, 1},
/* AF */ { &SM83::ADC_A_A, 4, 1},

/* B0 */ { &SM83::OR_A_B, 4, 1},
/* B1 */ { &SM83::OR_A_C, 4, 1},
/* B2 */ { &SM83::OR_A_D, 4, 1},
/* B3 */ { &SM83::OR_A_E, 4, 1},
/* B4 */ { &SM83::OR_A_H, 4, 1},
/* B5 */ { &SM83::OR_A_L, 4, 1},
/* B6 */ { &SM83::OR_A_HL, 8, 1},
/* B7 */ { &SM83::OR_A_A, 4, 1},
/* B8 */ { &SM83::CP_A_B, 4, 1},
/* B9 */ { &SM83::CP_A_C, 4, 1},
/* BA */ { &SM83::CP_A_D, 4, 1},
/* BB */ { &SM83::CP_A_E, 4, 1},
/* BC */ { &SM83::CP_A_H, 4, 1},
/* BD */ { &SM83::CP_A_L, 4, 1},
/* BE */ { &SM83::CP_A_HL, 8, 1},
/* BF */ { &SM83::CP_A_A, 4, 1},

/* C0 */ { &SM83::RET_NZ, 8, 1},
/* C1 */ { &SM83::POP_BC, 12, 1},
/* C2 */ { &SM83::JP_NZ, 12, 3},
/* C3 */ { &SM83::JP, 16, 3},
/* C4 */ { &SM83::CALL_NZ, 12, 3},
/* C5 */ { &SM83::PUSH_BC, 16, 1},
/* C6 */ { &SM83::ADD_A_n8, 8, 2},
/* C7 */ { &SM83::RST_00, 16, 1},
/* C8 */ { &SM83::RET_Z, 8, 1},
/* C9 */ { &SM83::RET, 16, 1},
/* CA */ { &SM83::JP_Z, 12, 3},
/* CB */ {},
/* CC */ { &SM83::CALL_Z, 12, 3},
/* CD */ { &SM83::CALL, 24, 3},
/* CE */ { &SM83::ADC_A_n8, 8, 2},
/* CF */ { &SM83::RST_08, 16, 1},

/* D0 */ { &SM83::RET_NC, 8, 1},
/* D1 */ { &SM83::POP_DE, 12, 1},
/* D2 */ { &SM83::JP_NC, 12, 3},
/* D3 */ {},
/* D4 */ { &SM83::CALL_NC, 12, 3},
/* D5 */ { &SM83::PUSH_DE, 16, 1},
/* D6 */ { &SM83::SUB_A, 8, 2},
/* D7 */ { &SM83::RST_10, 16, 1},
/* D8 */ { &SM83::RET_C, 8, 1},
/* D9 */ { &SM83::RETI, 16, 1},
/* DA */ { &SM83::JP_C, 12, 3},
/* DB */ {},
/* DC */ { &SM83::CALL_C, 12, 3},
/* DD */ {},
/* DE */ { &SM83::SBC_A_n8, 8, 2},
/* DF */ { &SM83::RST_18, 16, 1},

/* E0 */ { &SM83::LDH_A_n16, 4, 1},
/* E1 */ { &SM83::POP_HL, 12, 1},
/* E2 */ { &SM83::AND_A_D, 4, 1},
/* E3 */ {},
/* E4 */ {},
/* E5 */ { &SM83::PUSH_HL, 16, 1},
/* E6 */ { &SM83::AND_A, 8, 2},
/* E7 */ { &SM83::RST_20, 16, 1},
/* E8 */ { &SM83::ADD_SP_e8, 16, 2},
/* E9 */ { &SM83::JP_HL, 4, 1},
/* EA */ { &SM83::LD_addr_A, 16, 3},
/* EB */ {},
/* EC */ {},
/* ED */ {},
/* EE */ { &SM83::XOR_A, 8, 2},
/* EF */ { &SM83::RST_28, 16, 1},

/* F0 */ { &SM83::LDH_A_n16, 4, 1},
/* F1 */ { &SM83::POP_AF, 12, 1},
/* F2 */ { &SM83::AND_A_D, 4, 1},
/* F3 */ { &SM83::DI, 4, 1},
/* F4 */ {},
/* F5 */ { &SM83::PUSH_AF, 16, 1},
/* F6 */ { &SM83::OR_A_n8, 8, 2},
/* F7 */ { &SM83::RST_30, 16, 1},
/* F8 */ { &SM83::LD_HL_SP_e8, 12, 2},
/* F9 */ { &SM83::LD_SP_HL, 8, 1},
/* FA */ { &SM83::LD_A_n16, 16, 3},
/* FB */ { &SM83::EI, 4, 1},
/* FC */ {},
/* FD */ {},
/* FE */ { &SM83::CP_A_n8, 8, 2},
/* FF */ { &SM83::RST_38, 16, 1},
};

    static constexpr Opcode opcodeTableCB[256] = {
/* 00 */ { &SM83::RLC_B, 8, 2 },
/* 01 */ { &SM83::RLC_C, 8, 2 },
/* 02 */ { &SM83::RLC_D, 8, 2 },
/* 03 */ { &SM83::RLC_E, 8, 2 },
/* 04 */ { &SM83::RLC_H, 8, 2 },
/* 05 */ { &SM83::RLC_L, 8, 2 },
/* 06 */ { &SM83::RLC_HL, 16, 2 },
/* 07 */ { &SM83::RLC_A, 8, 2 },
/* 08 */ { &SM83::RRC_B, 8, 2 },
/* 09 */ { &SM83::RRC_C, 8, 2 },
/* 0A */ { &SM83::RRC_D, 8, 2 },
/* 0B */ { &SM83::RRC_E, 8, 2 },
/* 0C */ { &SM83::RRC_H, 8, 2 },
/* 0D */ { &SM83::RRC_L, 8, 2 },
/* 0E */ { &SM83::RRC_HL, 16, 2 },
/* 0F */ { &SM83::RRC_A, 8, 2 },

/* 10 */ { &SM83::RL_B, 8, 2 },
/* 11 */ { &SM83::RL_C, 8, 2 },
/* 12 */ { &SM83::RL_D, 8, 2 },
/* 13 */ { &SM83::RL_E, 8, 2 },
/* 14 */ { &SM83::RL_H, 8, 2 },
/* 15 */ { &SM83::RL_L, 8, 2 },
/* 16 */ { &SM83::RL_HL, 16, 2 },
/* 17 */ { &SM83::RL_A, 8, 2 },
/* 18 */ { &SM83::RR_B, 12, 2 },
/* 19 */ { &SM83::RR_C, 8, 2 },
/* 1A */ { &SM83::RR_D, 8, 2 },
/* 1B */ { &SM83::RR_E, 8, 2 },
/* 1C */ { &SM83::RR_H, 8, 2 },
/* 1D */ { &SM83::RR_L, 8, 2 },
/* 1E */ { &SM83::RR_HL, 16, 2 },
/* 1F */ { &SM83::RR_A, 8, 2 },

/* 20 */ { &SM83::SLA_B, 8, 2 },
/* 21 */ { &SM83::SLA_C, 8, 2 },
/* 22 */ { &SM83::SLA_D, 8, 2 },
/* 23 */ { &SM83::SLA_E, 8, 2 },
/* 24 */ { &SM83::SLA_H, 8, 2 },
/* 25 */ { &SM83::SLA_L, 8, 2 },
/* 26 */ { &SM83::SLA_HL, 16, 2 },
/* 27 */ { &SM83::SLA_A, 8, 2 },
/* 28 */ { &SM83::SRA_B, 8, 2 },
/* 29 */ { &SM83::SRA_C, 8, 2 },
/* 2A */ { &SM83::SRA_D, 8, 2 },
/* 2B */ { &SM83::SRA_E, 8, 2 },
/* 2C */ { &SM83::SRA_H, 8, 2 },
/* 2D */ { &SM83::SRA_L, 8, 2 },
/* 2E */ { &SM83::SRA_HL, 16, 2 },
/* 2F */ { &SM83::SRA_A, 8, 2 },

/* 30 */ { &SM83::SWAP_B, 8, 2 },
/* 31 */ { &SM83::SWAP_C, 8, 2 },
/* 32 */ { &SM83::SWAP_D, 8, 1 },
/* 33 */ { &SM83::SWAP_E, 8, 2 },
/* 34 */ { &SM83::SWAP_H, 8, 2 },
/* 35 */ { &SM83::SWAP_L, 8, 2 },
/* 36 */ { &SM83::SWAP_HL, 16, 2 },
/* 37 */ { &SM83::SWAP_A, 8, 2 },
/* 38 */ { &SM83::SRL_B, 8, 2 },
/* 39 */ { &SM83::SRL_C, 8, 2 },
/* 3A */ { &SM83::SRL_D, 8, 2 },
/* 3B */ { &SM83::SRL_E, 8, 2 },
/* 3C */ { &SM83::SRL_H, 8, 2 },
/* 3D */ { &SM83::SRL_L, 8, 2 },
/* 3E */ { &SM83::SRL_HL, 16, 2 },
/* 3F */ { &SM83::SRL_A, 8, 2 },

/* 40 */ { &SM83::BIT_0_B, 8, 2},
/* 41 */ { &SM83::BIT_0_C, 8, 2},
/* 42 */ { &SM83::BIT_0_D, 8, 2},
/* 43 */ { &SM83::BIT_0_E, 8, 2},
/* 44 */ { &SM83::BIT_0_H, 8, 2},
/* 45 */ { &SM83::BIT_0_L, 8, 2},
/* 46 */ { &SM83::BIT_0_HL, 12, 2},
/* 47 */ { &SM83::BIT_1_A, 8, 2},
/* 48 */ { &SM83::BIT_1_B, 8, 2},
/* 49 */ { &SM83::BIT_1_C, 8, 2},
/* 4A */ { &SM83::BIT_1_D, 8, 2},
/* 4B */ { &SM83::BIT_1_E, 8, 2},
/* 4C */ { &SM83::BIT_1_H, 8, 2},
/* 4D */ { &SM83::BIT_1_L, 8, 2},
/* 4E */ { &SM83::BIT_1_HL, 12, 2},
/* 4F */ { &SM83::BIT_1_A, 8, 2},

/* 50 */ { &SM83::BIT_2_B, 8, 2},
/* 51 */ { &SM83::BIT_2_C, 8, 2},
/* 52 */ { &SM83::BIT_2_D, 8, 2},
/* 53 */ { &SM83::BIT_2_E, 8, 2},
/* 54 */ { &SM83::BIT_2_H, 8, 2},
/* 55 */ { &SM83::BIT_2_L, 8, 2},
/* 56 */ { &SM83::BIT_2_HL, 16, 2},
/* 57 */ { &SM83::BIT_2_A, 8, 2},
/* 58 */ { &SM83::BIT_3_B, 8, 2},
/* 59 */ { &SM83::BIT_3_C, 8, 2},
/* 5A */ { &SM83::BIT_3_D, 8, 2},
/* 5B */ { &SM83::BIT_3_E, 8, 2},
/* 5C */ { &SM83::BIT_3_H, 8, 2},
/* 5D */ { &SM83::BIT_3_L, 8, 2},
/* 5E */ { &SM83::BIT_3_HL, 16, 2},
/* 5F */ { &SM83::BIT_3_A, 8, 2},

/* 60 */ { &SM83::BIT_4_B, 8, 2},
/* 61 */ { &SM83::BIT_4_C, 8, 2},
/* 62 */ { &SM83::BIT_4_D, 8, 2},
/* 63 */ { &SM83::BIT_4_E, 8, 2},
/* 64 */ { &SM83::BIT_4_H, 8, 2},
/* 65 */ { &SM83::BIT_4_L, 8, 2},
/* 66 */ { &SM83::BIT_4_HL, 16, 2},
/* 67 */ { &SM83::BIT_4_A, 8, 2},
/* 68 */ { &SM83::BIT_5_B, 8, 2},
/* 69 */ { &SM83::BIT_5_C, 8, 2},
/* 6A */ { &SM83::BIT_5_D, 8, 2},
/* 6B */ { &SM83::BIT_5_E, 8, 2},
/* 6C */ { &SM83::BIT_5_H, 8, 2},
/* 6D */ { &SM83::BIT_5_L, 8, 2},
/* 6E */ { &SM83::BIT_5_HL, 16, 2},
/* 6F */ { &SM83::BIT_5_A, 8, 2},

/* 70 */ { &SM83::BIT_6_B, 8, 2},
/* 71 */ { &SM83::BIT_6_C, 8, 2},
/* 72 */ { &SM83::BIT_6_D, 8, 2},
/* 73 */ { &SM83::BIT_6_E, 8, 2},
/* 74 */ { &SM83::BIT_6_H, 8, 2},
/* 75 */ { &SM83::BIT_6_L, 8, 2},
/* 76 */ { &SM83::BIT_6_HL, 16, 2},
/* 77 */ { &SM83::BIT_6_A, 8, 2},
/* 78 */ { &SM83::BIT_7_B, 8, 2},
/* 79 */ { &SM83::BIT_7_C, 8, 2},
/* 7A */ { &SM83::BIT_7_D, 8, 2},
/* 7B */ { &SM83::BIT_7_E, 8, 2},
/* 7C */ { &SM83::BIT_7_H, 8, 2},
/* 7D */ { &SM83::BIT_7_L, 8, 2},
/* 7E */ { &SM83::BIT_7_HL, 16, 2},
/* 7F */ { &SM83::BIT_7_A, 8, 2},

/* 80 */ { &SM83::RES_0_B,8,2 },
/* 81 */ { &SM83::RES_0_C,8,2 },
/* 82 */ { &SM83::RES_0_D,8,2 },
/* 83 */ { &SM83::RES_0_E,8,2 },
/* 84 */ { &SM83::RES_0_H,8,2 },
/* 85 */ { &SM83::RES_0_L,8,2 },
/* 86 */ { &SM83::RES_0_HL,16,2 },
/* 87 */ { &SM83::RES_0_A,8,2 },
/* 88 */ { &SM83::RES_1_B,8,2 },
/* 89 */ { &SM83::RES_1_C,8,2 },
/* 8A */ { &SM83::RES_1_D,8,2 },
/* 8B */ { &SM83::RES_1_E,8,2 },
/* 8C */ { &SM83::RES_1_H,8,2 },
/* 8D */ { &SM83::RES_1_L,8,2 },
/* 8E */ { &SM83::RES_1_HL,16,2 },
/* 8F */ { &SM83::RES_1_A,8,2 },

/* 90 */ { &SM83::RES_2_B,8,2 },
/* 91 */ { &SM83::RES_2_C,8,2 },
/* 92 */ { &SM83::RES_2_D,8,2 },
/* 93 */ { &SM83::RES_2_E,8,2 },
/* 94 */ { &SM83::RES_2_H,8,2 },
/* 95 */ { &SM83::RES_2_L,8,2 },
/* 96 */ { &SM83::RES_2_HL,16,2 },
/* 97 */ { &SM83::RES_2_A,8,2 },
/* 98 */ { &SM83::RES_3_B,8,2 },
/* 99 */ { &SM83::RES_3_C,8,2 },
/* 9A */ { &SM83::RES_3_D,8,2 },
/* 9B */ { &SM83::RES_3_E,8,2 },
/* 9C */ { &SM83::RES_3_H,8,2 },
/* 9D */ { &SM83::RES_3_L,8,2 },
/* 9E */ { &SM83::RES_3_HL,16,2 },
/* 9F */ { &SM83::RES_3_A,8,2 },

/* A0 */ { &SM83::RES_4_B, 8, 2},
/* A1 */ { &SM83::RES_4_C, 8, 2},
/* A2 */ { &SM83::RES_4_D, 8, 2},
/* A3 */ { &SM83::RES_4_E, 8, 2},
/* A4 */ { &SM83::RES_4_H, 8, 2},
/* A5 */ { &SM83::RES_4_L, 8, 2},
/* A6 */ { &SM83::RES_4_HL, 16, 2},
/* A7 */ { &SM83::RES_4_A, 8, 2},
/* A8 */ { &SM83::RES_5_B, 8, 2},
/* A9 */ { &SM83::RES_5_C, 8, 2},
/* AA */ { &SM83::RES_5_D, 8, 2},
/* AB */ { &SM83::RES_5_E, 8, 2},
/* AC */ { &SM83::RES_5_H, 8, 2},
/* AD */ { &SM83::RES_5_L, 8, 2},
/* AE */ { &SM83::RES_5_HL, 16, 2},
/* AF */ { &SM83::RES_5_A, 8, 2},

/* B0 */ { &SM83::RES_6_B, 8, 2},
/* B1 */ { &SM83::RES_6_C, 8, 2},
/* B2 */ { &SM83::RES_6_D, 8, 2},
/* B3 */ { &SM83::RES_6_E, 8, 2},
/* B4 */ { &SM83::RES_6_H, 8, 2},
/* B5 */ { &SM83::RES_6_L, 8, 2},
/* B6 */ { &SM83::RES_6_HL, 16, 2},
/* B7 */ { &SM83::RES_6_A, 8, 2},
/* B8 */ { &SM83::RES_7_B, 8, 2},
/* B9 */ { &SM83::RES_7_C, 8, 2},
/* BA */ { &SM83::RES_7_D, 8, 2},
/* BB */ { &SM83::RES_7_E, 8, 2},
/* BC */ { &SM83::RES_7_H, 8, 2},
/* BD */ { &SM83::RES_7_L, 8, 2},
/* BE */ { &SM83::RES_7_HL, 16, 2},
/* BF */ { &SM83::RES_7_A, 8, 2},

/* C0 */ { &SM83::SET_0_B, 8, 2},
/* C1 */ { &SM83::SET_0_C, 8, 2},
/* C2 */ { &SM83::SET_0_D, 8, 2},
/* C3 */ { &SM83::SET_0_E, 8, 2},
/* C4 */ { &SM83::SET_0_H, 8, 2},
/* C5 */ { &SM83::SET_0_L, 8, 2},
/* C6 */ { &SM83::SET_0_HL, 16, 2},
/* C7 */ { &SM83::SET_0_A, 8, 2},
/* C8 */ { &SM83::SET_1_B, 8, 2},
/* C9 */ { &SM83::SET_1_C, 8, 2},
/* CA */ { &SM83::SET_1_D, 8, 2},
/* CB */ { &SM83::SET_1_E, 8, 2},
/* CC */ { &SM83::SET_1_H, 8, 2},
/* CD */ { &SM83::SET_1_L, 8, 2},
/* CE */ { &SM83::SET_1_HL, 16, 2},
/* CF */ { &SM83::SET_1_A, 8, 2},

/* D0 */ { &SM83::SET_2_B, 8, 2},
/* D1 */ { &SM83::SET_2_C, 8, 2},
/* D2 */ { &SM83::SET_2_D, 8, 2},
/* D3 */ { &SM83::SET_2_E, 8, 2},
/* D4 */ { &SM83::SET_2_H, 8, 2},
/* D5 */ { &SM83::SET_2_L, 8, 2},
/* D6 */ { &SM83::SET_2_HL, 16, 2},
/* D7 */ { &SM83::SET_2_A, 8, 2},
/* D8 */ { &SM83::SET_3_B, 8, 2},
/* D9 */ { &SM83::SET_3_C, 8, 2},
/* DA */ { &SM83::SET_3_D, 8, 2},
/* DB */ { &SM83::SET_3_E, 8, 2},
/* DC */ { &SM83::SET_3_H, 8, 2},
/* DD */ { &SM83::SET_3_L, 8, 2},
/* DE */ { &SM83::SET_3_HL, 16, 2},
/* DF */ { &SM83::SET_3_A, 8, 2},

/* E0 */ { &SM83::SET_4_B, 8, 2},
/* E1 */ { &SM83::SET_4_C, 8, 2},
/* E2 */ { &SM83::SET_4_D, 8, 2},
/* E3 */ { &SM83::SET_4_E, 8, 2},
/* E4 */ { &SM83::SET_4_H, 8, 2},
/* E5 */ { &SM83::SET_4_L, 8, 2},
/* E6 */ { &SM83::SET_4_HL, 16, 2},
/* E7 */ { &SM83::SET_4_A, 8, 2},
/* E8 */ { &SM83::SET_5_B, 8, 2},
/* E9 */ { &SM83::SET_5_C, 8, 2},
/* EA */ { &SM83::SET_5_D, 8, 2},
/* EB */ { &SM83::SET_5_E, 8, 2},
/* EC */ { &SM83::SET_5_H, 8, 2},
/* ED */ { &SM83::SET_5_L, 8, 2},
/* EE */ { &SM83::SET_5_HL, 16, 2},
/* EF */ { &SM83::SET_5_A, 8, 2},

/* F0 */ { &SM83::SET_6_B, 8, 2},
/* F1 */ { &SM83::SET_6_C, 8, 2},
/* F2 */ { &SM83::SET_6_D, 8, 2},
/* F3 */ { &SM83::SET_6_E, 8, 2},
/* F4 */ { &SM83::SET_6_H, 8, 2},
/* F5 */ { &SM83::SET_6_L, 8, 2},
/* F6 */ { &SM83::SET_6_HL, 16, 2},
/* F7 */ { &SM83::SET_6_A, 8, 2},
/* F8 */ { &SM83::SET_7_B, 8, 2},
/* F9 */ { &SM83::SET_7_C, 8, 2},
/* FA */ { &SM83::SET_7_D, 8, 2},
/* FB */ { &SM83::SET_7_E, 8, 2},
/* FC */ { &SM83::SET_7_H, 8, 2},
/* FD */ { &SM83::SET_7_L, 8, 2},
/* FE */ { &SM83::SET_7_HL, 16, 2},
/* FF */ { &SM83::SET_7_A, 8, 2},
};

};