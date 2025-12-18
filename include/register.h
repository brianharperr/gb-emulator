#pragma once
#include <cstdint>

inline auto check_bit(const uint8_t value, const uint8_t bit) -> bool { return (value & (1 << bit)) != 0; }

inline auto set_bit(const uint8_t value, const uint8_t bit) -> uint8_t {
    auto value_set = value | (1 << bit);
    return static_cast<uint8_t>(value_set);
}

inline auto clear_bit(const uint8_t value, const uint8_t bit) -> uint8_t {
    auto value_cleared = value & ~(1 << bit);
    return static_cast<uint8_t>(value_cleared);
}

class ByteRegister{
public:
    ByteRegister() = default;
    ByteRegister(uint8_t v) { val = v; }; 
    virtual void Set(const uint8_t value);
    uint8_t Get() const { return val; };
    void Increment();
    void Decrement();
protected:
    uint8_t val = 0x0;
};

class FlagRegister : public ByteRegister{
public:
    FlagRegister(uint8_t v) { val = v; }; 
    void Set(const uint8_t value) override;

    void SetZero(bool set);
    void SetSubtract(bool set);
    void SetHalfCarry(bool set);
    void SetCarry(bool set);

    bool Zero();
    bool Subtract();
    bool HalfCarry();
    bool Carry();
};

class WordRegister{
public:
    WordRegister(uint16_t v) { val = v; }; 
    void Set(const uint16_t value);
    uint16_t Get() const { return val; };
    void Increment();
    void Decrement();
protected:
    uint16_t val = 0x0;
};

