#include "CPU.h"

#include "MMU.h"

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
    /*0x04*/ WrapFunc(cpu.INC(cpu.registers.b)),
    /*0x05*/ WrapFunc(cpu.DEC(cpu.registers.b)),
    /*0x06*/ WrapFunc(cpu.LoadRegData8(cpu.registers.b)),
    /*0x07*/ WrapFunc(cpu.RLCA()),
    /*0x08*/ WrapFunc(cpu.LoadAddrSP()),
    /*0x09*/ WrapFunc(cpu.ADD(cpu.registers.hl, cpu.registers.bc)),
    /*0x0A*/ WrapFunc(cpu.LD(cpu.registers.a, Address(cpu.registers.bc))),
    /*0x0B*/ WrapFunc(cpu.DEC(cpu.registers.bc)),
    /*0x0C*/ WrapFunc(cpu.INC(cpu.registers.c)),
    /*0x0D*/ WrapFunc(cpu.DEC(cpu.registers.c)),
    /*0x0E*/ WrapFunc(cpu.LoadRegData8(cpu.registers.c)),
    /*0x0F*/ WrapFunc(cpu.RRCA()),

    /*0x10*/ WrapFunc(cpu.STOP()),
    /*0x11*/ WrapFunc(cpu.LoadRegData16(cpu.registers.de)),
    /*0x12*/ WrapFunc(cpu.LoadAddrRegA(cpu.registers.de)),
    /*0x13*/ WrapFunc(cpu.INC(cpu.registers.de)),
    /*0x14*/ WrapFunc(cpu.INC(cpu.registers.d)),
    /*0x15*/ WrapFunc(cpu.DEC(cpu.registers.d)),
    /*0x16*/ WrapFunc(cpu.LoadRegData8(cpu.registers.d)),
    /*0x17*/ WrapFunc(cpu.RLA()),
    /*0x18*/ WrapFunc(cpu.JR()),
    /*0x19*/ WrapFunc(cpu.ADD(cpu.registers.hl, cpu.registers.de)),
    /*0x1A*/ WrapFunc(cpu.LD(cpu.registers.a, Address(cpu.registers.de))),
    /*0x1B*/ WrapFunc(cpu.DEC(cpu.registers.de)),
    /*0x1C*/ WrapFunc(cpu.INC(cpu.registers.e)),
    /*0x1D*/ WrapFunc(cpu.DEC(cpu.registers.e)),
    /*0x1E*/ WrapFunc(cpu.LoadRegData8(cpu.registers.e)),
    /*0x1F*/ WrapFunc(cpu.RRA()),
    
    /*0x20*/ WrapFunc(cpu.JR(!cpu.registers.f.Z)),
    /*0x21*/ WrapFunc(cpu.LoadRegData16(cpu.registers.hl)),
    /*0x22*/ WrapFunc(cpu.LoadAddrRegA(cpu.registers.hl++)),
    /*0x23*/ WrapFunc(cpu.INC(cpu.registers.hl)),
    /*0x24*/ WrapFunc(cpu.INC(cpu.registers.h)),
    /*0x25*/ WrapFunc(cpu.DEC(cpu.registers.h)),
    /*0x26*/ WrapFunc(cpu.LoadRegData8(cpu.registers.h)),

    }
{
    registers.af = 0x01B0;
    registers.bc = 0x0013;
    registers.de = 0x00D8;
    registers.hl = 0x014D;
    registers.sp = 0xFFFE;
    registers.pc = 0x0100;
}

uint8 CPU::GetByteFromPC()
{
    return memory.ReadByte(registers.pc++);
}

uint16 CPU::GetWordFromPC()
{
    const uint16 data = memory.ReadWord(registers.pc);
    registers.pc += 2;
    return data;
}

int8 CPU::GetSignedByteFromPC()
{
    return static_cast<int8>(GetByteFromPC());
}

int16 CPU::GetSignedWordFromPC()
{
    return static_cast<int16>(GetWordFromPC());
}

}