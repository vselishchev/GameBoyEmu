#pragma once

namespace GameBoyEmu
{

using uint8 = unsigned char;
using uint16 = unsigned short;

struct MMU
{
union
{
    uint8 memory[0x10000];
    struct
    {
        uint8 bank0[0x4000];
        uint8 bankN[0x4000];
        uint8 videoRAM[0x2000];
        uint8 externalRAM[0x2000];
        uint8 workRAM[0x2000];
        uint8 shadowWorkRAM[0x1E00]; // (Echo RAM) Why it's not equal to workRAM? Not used. Prohibited area.
        uint8 oam[0x00A0]; // SpriteAttributeTable
        uint8 blob[0x0060]; // Not used. Prohibited area.
        uint8 ioRegisters[0x0080];
        uint8 highRAM[0x007F];
        uint8 interruptRegister; // IE. Interrupt Enable register.
    };
};

};

} // GameBoyEmu