#pragma once

#include "Utils.h"

namespace GameBoyEmu
{

struct MMU;

struct FlagRegister
{
    bool Zero : 1;
    bool N : 1; // Add/sub flags (BCD)
    bool H : 1; // Half carry flag (BCD)
    bool C : 1; // Carry flag
    bool UNUSED : 4; // Unused, always zero
};

struct Registers
{
    union 
    {
        uint16 af;
        struct
        {
            FlagRegister f;
            uint8 a;
        };
    };
    union
    {
        uint16 bc;
        struct
        {
            uint8 c;
            uint8 b;
        };
    };
    union
    {
        uint16 de;
        struct
        {
            uint8 e;
            uint8 d;
        };
    };
    union
    {
        uint16 hl;
        struct
        {
            uint8 l;
            uint8 h;
        };
    };
    uint16 sp; // Stack pointer.
    uint16 pc; // Program counter.
};

class CPU
{
public:

private:
    CPU(MMU& mmu);
    void Tick(uint8 ticks = 1) {} // TODO

    // Opcodes
    void LoadRegData16(uint16& reg); // Load word data into the word register.
    void LoadAddrRegA(Address address); // Load A (accumulator) register into the address stored in the word register.

    void CBOpcodeFunc();

    /// Instructions
    void NOP() { Tick();} // Takes CPU cycles.
    void None() {} // Called on invalid instruction.

    // Load
    void LD(uint8& to, uint8 from);
    void LD(uint16& to, uint16 from);
    void LD(uint8& to, Address address);
    void LD(uint16& to, Address address);
    void LD(Address address, uint8 from);
    void LD(Address address, uint16 from);

    // Increment
    void INC(uint8& value);
    void INC(uint16& value);
    void INC(Address address);

private:
    MMU& memory;
    Registers registers;

    using OpcodeFunc = void(*)(CPU&);
    OpcodeFunc opcodesTable[0x100];
    OpcodeFunc CBOpcodeTable[0x100]; // This upcode table accessed if first byte of opcode was 0xCB.
};

}