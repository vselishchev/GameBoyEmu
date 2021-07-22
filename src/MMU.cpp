#include "MMU.h"

namespace GameBoyEmu
{

MMU& MMU::GetMMU()
{
    static MMU mmu;
    return mmu;
}

MMU::MMU()
    : isBooted(&memory[0xFF50])
{
}

uint8 MMU::ReadByte(uint16 address)
{
    if (*isBooted == 0 && address < 0x0100)
    {
        return bios[address];
    }

    if (address > 0xDFFF && address < 0xFE00)
    {
        return memory[address - 0x2000];
    }
    else
    {
        return memory[address];
    }
}

void MMU::WriteByte(uint16 address, uint8 value)
{
    if (*isBooted == 0 && address < 0x0100)
    {
        return; // can't write into the BootROM
    }

    if (address > 0xDFFF && address < 0xFE00)
    {
        // ECHO RAM. Put into the actual ram.
        memory[address - 0x2000] = value;
    }
    else
    {
        memory[address] = value;
    }
}

uint16 MMU::ReadWord(uint16 address)
{
    if (*isBooted == 0 && address < 0x0100)
    {
        return (uint16)bios[address] << 8u | bios[address + 1];
    }

    uint16 finalAddress = address;
    if (address > 0xDFFF && address < 0xFE00)
    {
        finalAddress -= 0x2000;
    }

    return (uint16)memory[finalAddress] << 8u | memory[finalAddress + 1];
}

void MMU::WriteWord(uint16 address, uint16 value)
{
    if (*isBooted == 0 && address < 0x0100)
    {
        bios[address] = (value & 0xFF00) >> 8u;
        bios[address + 1] = value & 0x00FF;
    }

    uint16 finalAddress = address;
    if (address > 0xDFFF && address < 0xFE00)
    {
        finalAddress -= 0x2000;
    }

    memory[finalAddress] = (value & 0xFF00) >> 8u;
    memory[finalAddress + 1] = value & 0x00FF;
}

}