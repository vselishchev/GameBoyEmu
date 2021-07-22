#pragma once

#include "Utils.h"

namespace GameBoyEmu
{

class MMU
{
public:
    static MMU& GetMMU();

    uint8 ReadByte(uint16 address);
    void WriteByte(uint16 address, uint8 value);

    uint16 ReadWord(uint16 address);
    void WriteWord(uint16 address, uint16 value);

private:
    MMU();
    union
    {
        uint8 memory[0x10000];
        struct
        {
            uint8 bank0[BankSize];
            uint8 bankN[BankSize]; // switchable bank.
            uint8 videoRAM[VideoRAMSize];
            uint8 externalRAM[ExternalRAMSize]; // cartridge storage. Used for savings.
            uint8 workRAM[WorkRAMSize];
            uint8 shadowWorkRAM[ShadowWorkRAMSize]; // (Echo RAM) Why it's not equal to workRAM? Not used. Prohibited area.
            uint8 oam[OAMSize]; // SpriteAttributeTable
            uint8 blob[BlobSize]; // Not used. Prohibited area.
            uint8 ioRegisters[IORegistersSize];
            uint8 highRAM[HighRAMSize];
            uint8 interruptRegister; // IE. Interrupt Enable register.
        };
    };
    uint8 bios[0x100]; // BootROM
    uint8* isBooted;
};

} // GameBoyEmu