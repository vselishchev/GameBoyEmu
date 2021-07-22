#include "CPU.h"

namespace GameBoyEmu
{

CPU::CPU(MMU& mmu)
    : memory(mmu)
{
    registers.af = 0x01B0;
    registers.bc = 0x0013;
    registers.de = 0x00D8;
    registers.hl = 0x014D;
    registers.sp = 0xFFFE;
    registers.pc = 0x0100;
}

}