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
    CPU(MMU& mmu);

private:
    void OpcodeLD(uint8& to, uint8 from);
    void OpcodeLD(uint8& to, Address address);
    void OpcodeLD(uint16& to, uint16 from);
    void OpcodeLD(uint16& to, Address address);
    void OpcodeLD(Address address, uint8 from);

private:
    MMU& memory;
    Registers registers;
};

}