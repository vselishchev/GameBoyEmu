#pragma once

#include "Utils.h"

namespace GameBoyEmu
{

struct MMU;

struct Registers
{
    union 
    {
        uint16 af;
        struct
        {
            uint8 f; // Flags register.
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
    CPU(MMU& memory);

private:
    MMU& memory;
    Registers registers;
};

}