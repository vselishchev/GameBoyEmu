#include "CPU.h"



namespace GameBoyEmu
{

#define WrapFunc(...) \
    [](CPU& cpu) { __VA_ARGS__; }

CPU::CPU(MMU& mmu)
    : memory(mmu)
    , opcodesTable
    {
    /*0x00*/ WrapFunc(cpu.NOP()), //](CPU& cpu) { cpu.NOP(); },
    /*0x01*/ WrapFunc(cpu.LoadRegData16(cpu.registers.bc)),
    /*0x02*/ WrapFunc(cpu.LoadAddrRegA(cpu.registers.bc)),
    /*0x03*/ WrapFunc(cpu.INC(cpu.registers.bc)),
    /*0x04*/ WrapFunc(cpu.INC(cpu.registers.b))
    }
{
    registers.af = 0x01B0;
    registers.bc = 0x0013;
    registers.de = 0x00D8;
    registers.hl = 0x014D;
    registers.sp = 0xFFFE;
    registers.pc = 0x0100;
}

}