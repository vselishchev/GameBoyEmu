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
    /*0x00*/ WrapFunc(cpu.NOP()),
    /*0x01*/ WrapFunc(cpu.LoadRegData16(cpu.registers.bc)),
    /*0x02*/ WrapFunc(cpu.LD(Address(cpu.registers.bc), cpu.registers.a)),
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
    /*0x12*/ WrapFunc(cpu.LD(Address(cpu.registers.de), cpu.registers.a)),
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
    /*0x22*/ WrapFunc(cpu.LD(Address(cpu.registers.hl++), cpu.registers.a)),
    /*0x23*/ WrapFunc(cpu.INC(cpu.registers.hl)),
    /*0x24*/ WrapFunc(cpu.INC(cpu.registers.h)),
    /*0x25*/ WrapFunc(cpu.DEC(cpu.registers.h)),
    /*0x26*/ WrapFunc(cpu.LoadRegData8(cpu.registers.h)),
    /*0x27*/ WrapFunc(cpu.DAA()),
    /*0x28*/ WrapFunc(cpu.JR(cpu.registers.f.Z)),
    /*0x29*/ WrapFunc(cpu.ADD(cpu.registers.hl, cpu.registers.hl)),
    /*0x2A*/ WrapFunc(cpu.LD(cpu.registers.a, Address(cpu.registers.hl++))),
    /*0x2B*/ WrapFunc(cpu.DEC(cpu.registers.hl)),
    /*0x2C*/ WrapFunc(cpu.INC(cpu.registers.l)),
    /*0x2D*/ WrapFunc(cpu.DEC(cpu.registers.l)),
    /*0x2E*/ WrapFunc(cpu.LoadRegData8(cpu.registers.l)),
    /*0x2F*/ WrapFunc(cpu.CPL()),

    /*0x30*/ WrapFunc(cpu.JR(!cpu.registers.f.C)),
    /*0x31*/ WrapFunc(cpu.LoadRegData16(cpu.registers.sp)),
    /*0x32*/ WrapFunc(cpu.LD(Address(cpu.registers.hl--), cpu.registers.a)),
    /*0x33*/ WrapFunc(cpu.INC(cpu.registers.sp)),
    /*0x34*/ WrapFunc(cpu.INC(Address(cpu.registers.hl))),
    /*0x35*/ WrapFunc(cpu.DEC(Address(cpu.registers.hl))),
    /*0x36*/ WrapFunc(cpu.LoadAddrData8(Address(cpu.registers.hl))),
    /*0x37*/ WrapFunc(cpu.SCF()),
    /*0x38*/ WrapFunc(cpu.JR(cpu.registers.f.C)),
    /*0x39*/ WrapFunc(cpu.ADD(cpu.registers.hl, cpu.registers.sp)),
    /*0x3A*/ WrapFunc(cpu.LD(cpu.registers.a, Address(cpu.registers.hl--))),
    /*0x3B*/ WrapFunc(cpu.DEC(cpu.registers.sp)),
    /*0x3C*/ WrapFunc(cpu.INC(cpu.registers.a)),
    /*0x3D*/ WrapFunc(cpu.DEC(cpu.registers.a)),
    /*0x3E*/ WrapFunc(cpu.LoadRegData8(cpu.registers.a)),
    /*0x3F*/ WrapFunc(cpu.CCF()),

    /*0x40*/ WrapFunc(cpu.LD(cpu.registers.b, cpu.registers.b)),
    /*0x41*/ WrapFunc(cpu.LD(cpu.registers.b, cpu.registers.c)),
    /*0x42*/ WrapFunc(cpu.LD(cpu.registers.b, cpu.registers.d)),
    /*0x43*/ WrapFunc(cpu.LD(cpu.registers.b, cpu.registers.e)),
    /*0x44*/ WrapFunc(cpu.LD(cpu.registers.b, cpu.registers.h)),
    /*0x45*/ WrapFunc(cpu.LD(cpu.registers.b, cpu.registers.l)),
    /*0x46*/ WrapFunc(cpu.LD(cpu.registers.b, Address(cpu.registers.hl))),
    /*0x47*/ WrapFunc(cpu.LD(cpu.registers.b, cpu.registers.a)),
    /*0x48*/ WrapFunc(cpu.LD(cpu.registers.c, cpu.registers.b)),
    /*0x49*/ WrapFunc(cpu.LD(cpu.registers.c, cpu.registers.c)),
    /*0x4A*/ WrapFunc(cpu.LD(cpu.registers.c, cpu.registers.d)),
    /*0x4B*/ WrapFunc(cpu.LD(cpu.registers.c, cpu.registers.e)),
    /*0x4C*/ WrapFunc(cpu.LD(cpu.registers.c, cpu.registers.h)),
    /*0x4D*/ WrapFunc(cpu.LD(cpu.registers.c, cpu.registers.l)),
    /*0x4E*/ WrapFunc(cpu.LD(cpu.registers.c, Address(cpu.registers.hl))),
    /*0x4F*/ WrapFunc(cpu.LD(cpu.registers.c, cpu.registers.a)),

    /*0x50*/ WrapFunc(cpu.LD(cpu.registers.d, cpu.registers.b)),
    /*0x51*/ WrapFunc(cpu.LD(cpu.registers.d, cpu.registers.c)),
    /*0x52*/ WrapFunc(cpu.LD(cpu.registers.d, cpu.registers.d)),
    /*0x53*/ WrapFunc(cpu.LD(cpu.registers.d, cpu.registers.e)),
    /*0x54*/ WrapFunc(cpu.LD(cpu.registers.d, cpu.registers.h)),
    /*0x55*/ WrapFunc(cpu.LD(cpu.registers.d, cpu.registers.l)),
    /*0x56*/ WrapFunc(cpu.LD(cpu.registers.d, Address(cpu.registers.hl))),
    /*0x57*/ WrapFunc(cpu.LD(cpu.registers.d, cpu.registers.a)),
    /*0x58*/ WrapFunc(cpu.LD(cpu.registers.e, cpu.registers.b)),
    /*0x59*/ WrapFunc(cpu.LD(cpu.registers.e, cpu.registers.c)),
    /*0x5A*/ WrapFunc(cpu.LD(cpu.registers.e, cpu.registers.d)),
    /*0x5B*/ WrapFunc(cpu.LD(cpu.registers.e, cpu.registers.e)),
    /*0x5C*/ WrapFunc(cpu.LD(cpu.registers.e, cpu.registers.h)),
    /*0x5D*/ WrapFunc(cpu.LD(cpu.registers.e, cpu.registers.l)),
    /*0x5E*/ WrapFunc(cpu.LD(cpu.registers.e, Address(cpu.registers.hl))),
    /*0x5F*/ WrapFunc(cpu.LD(cpu.registers.e, cpu.registers.a)),

    /*0x60*/ WrapFunc(cpu.LD(cpu.registers.h, cpu.registers.b)),
    /*0x61*/ WrapFunc(cpu.LD(cpu.registers.h, cpu.registers.c)),
    /*0x62*/ WrapFunc(cpu.LD(cpu.registers.h, cpu.registers.d)),
    /*0x63*/ WrapFunc(cpu.LD(cpu.registers.h, cpu.registers.e)),
    /*0x64*/ WrapFunc(cpu.LD(cpu.registers.h, cpu.registers.h)),
    /*0x65*/ WrapFunc(cpu.LD(cpu.registers.h, cpu.registers.l)),
    /*0x66*/ WrapFunc(cpu.LD(cpu.registers.h, Address(cpu.registers.hl))),
    /*0x67*/ WrapFunc(cpu.LD(cpu.registers.h, cpu.registers.a)),
    /*0x68*/ WrapFunc(cpu.LD(cpu.registers.l, cpu.registers.b)),
    /*0x69*/ WrapFunc(cpu.LD(cpu.registers.l, cpu.registers.c)),
    /*0x6A*/ WrapFunc(cpu.LD(cpu.registers.l, cpu.registers.d)),
    /*0x6B*/ WrapFunc(cpu.LD(cpu.registers.l, cpu.registers.e)),
    /*0x6C*/ WrapFunc(cpu.LD(cpu.registers.l, cpu.registers.h)),
    /*0x6D*/ WrapFunc(cpu.LD(cpu.registers.l, cpu.registers.l)),
    /*0x6E*/ WrapFunc(cpu.LD(cpu.registers.l, Address(cpu.registers.hl))),
    /*0x6F*/ WrapFunc(cpu.LD(cpu.registers.l, cpu.registers.a)),

    /*0x70*/ WrapFunc(cpu.LD(Address(cpu.registers.hl), cpu.registers.b)),
    /*0x71*/ WrapFunc(cpu.LD(Address(cpu.registers.hl), cpu.registers.c)),
    /*0x72*/ WrapFunc(cpu.LD(Address(cpu.registers.hl), cpu.registers.d)),
    /*0x73*/ WrapFunc(cpu.LD(Address(cpu.registers.hl), cpu.registers.e)),
    /*0x74*/ WrapFunc(cpu.LD(Address(cpu.registers.hl), cpu.registers.h)),
    /*0x75*/ WrapFunc(cpu.LD(Address(cpu.registers.hl), cpu.registers.l)),
    /*0x76*/ WrapFunc(cpu.HALT()),
    /*0x77*/ WrapFunc(cpu.LD(Address(cpu.registers.hl), cpu.registers.a)),
    /*0x78*/ WrapFunc(cpu.LD(cpu.registers.a, cpu.registers.b)),
    /*0x79*/ WrapFunc(cpu.LD(cpu.registers.a, cpu.registers.c)),
    /*0x7A*/ WrapFunc(cpu.LD(cpu.registers.a, cpu.registers.d)),
    /*0x7B*/ WrapFunc(cpu.LD(cpu.registers.a, cpu.registers.e)),
    /*0x7C*/ WrapFunc(cpu.LD(cpu.registers.a, cpu.registers.h)),
    /*0x7D*/ WrapFunc(cpu.LD(cpu.registers.a, cpu.registers.l)),
    /*0x7E*/ WrapFunc(cpu.LD(cpu.registers.a, Address(cpu.registers.hl))),
    /*0x7F*/ WrapFunc(cpu.LD(cpu.registers.a, cpu.registers.a)),

    /*0x80*/ WrapFunc(cpu.ADD(cpu.registers.a, cpu.registers.b)),
    /*0x81*/ WrapFunc(cpu.ADD(cpu.registers.a, cpu.registers.c)),
    /*0x82*/ WrapFunc(cpu.ADD(cpu.registers.a, cpu.registers.d)),
    /*0x83*/ WrapFunc(cpu.ADD(cpu.registers.a, cpu.registers.e)),
    /*0x84*/ WrapFunc(cpu.ADD(cpu.registers.a, cpu.registers.h)),
    /*0x85*/ WrapFunc(cpu.ADD(cpu.registers.a, cpu.registers.l)),
    /*0x86*/ WrapFunc(cpu.ADD(cpu.registers.a, Address(cpu.registers.hl))),
    /*0x87*/ WrapFunc(cpu.ADD(cpu.registers.a, cpu.registers.a)),
    /*0x88*/ WrapFunc(cpu.ADC(cpu.registers.a, cpu.registers.b)),
    /*0x89*/ WrapFunc(cpu.ADC(cpu.registers.a, cpu.registers.c)),
    /*0x8A*/ WrapFunc(cpu.ADC(cpu.registers.a, cpu.registers.d)),
    /*0x8B*/ WrapFunc(cpu.ADC(cpu.registers.a, cpu.registers.e)),
    /*0x8C*/ WrapFunc(cpu.ADC(cpu.registers.a, cpu.registers.h)),
    /*0x8D*/ WrapFunc(cpu.ADC(cpu.registers.a, cpu.registers.l)),
    /*0x8E*/ WrapFunc(cpu.ADC(cpu.registers.a, Address(cpu.registers.hl))),
    /*0x8F*/ WrapFunc(cpu.ADC(cpu.registers.a, cpu.registers.a)),

    /*0x90*/ WrapFunc(cpu.SUB(cpu.registers.b)),
    /*0x91*/ WrapFunc(cpu.SUB(cpu.registers.c)),
    /*0x92*/ WrapFunc(cpu.SUB(cpu.registers.d)),
    /*0x93*/ WrapFunc(cpu.SUB(cpu.registers.e)),
    /*0x94*/ WrapFunc(cpu.SUB(cpu.registers.h)),
    /*0x95*/ WrapFunc(cpu.SUB(cpu.registers.l)),
    /*0x96*/ WrapFunc(cpu.SUB(Address(cpu.registers.hl))),
    /*0x97*/ WrapFunc(cpu.SUB(cpu.registers.a)),
    /*0x98*/ WrapFunc(cpu.SBC(cpu.registers.b)),
    /*0x99*/ WrapFunc(cpu.SBC(cpu.registers.c)),
    /*0x9A*/ WrapFunc(cpu.SBC(cpu.registers.d)),
    /*0x9B*/ WrapFunc(cpu.SBC(cpu.registers.e)),
    /*0x9C*/ WrapFunc(cpu.SBC(cpu.registers.h)),
    /*0x9D*/ WrapFunc(cpu.SBC(cpu.registers.l)),
    /*0x9E*/ WrapFunc(cpu.SBC(Address(cpu.registers.hl))),
    /*0x9F*/ WrapFunc(cpu.SBC(cpu.registers.a)),

    /*0xA0*/ WrapFunc(cpu.AND(cpu.registers.b)),
    /*0xA1*/ WrapFunc(cpu.AND(cpu.registers.c)),
    /*0xA2*/ WrapFunc(cpu.AND(cpu.registers.d)),
    /*0xA3*/ WrapFunc(cpu.AND(cpu.registers.e)),
    /*0xA4*/ WrapFunc(cpu.AND(cpu.registers.h)),
    /*0xA5*/ WrapFunc(cpu.AND(cpu.registers.l)),
    /*0xA6*/ WrapFunc(cpu.AND(Address(cpu.registers.hl))),
    /*0xA7*/ WrapFunc(cpu.AND(cpu.registers.a)),
    /*0xA8*/ WrapFunc(cpu.XOR(cpu.registers.b)),
    /*0xA9*/ WrapFunc(cpu.XOR(cpu.registers.c)),
    /*0xAA*/ WrapFunc(cpu.XOR(cpu.registers.d)),
    /*0xAB*/ WrapFunc(cpu.XOR(cpu.registers.e)),
    /*0xAC*/ WrapFunc(cpu.XOR(cpu.registers.h)),
    /*0xAD*/ WrapFunc(cpu.XOR(cpu.registers.l)),
    /*0xAE*/ WrapFunc(cpu.XOR(Address(cpu.registers.hl))),
    /*0xAF*/ WrapFunc(cpu.XOR(cpu.registers.a)),

    /*0xB0*/ WrapFunc(cpu.OR(cpu.registers.b)),
    /*0xB1*/ WrapFunc(cpu.OR(cpu.registers.c)),
    /*0xB2*/ WrapFunc(cpu.OR(cpu.registers.d)),
    /*0xB3*/ WrapFunc(cpu.OR(cpu.registers.e)),
    /*0xB4*/ WrapFunc(cpu.OR(cpu.registers.h)),
    /*0xB5*/ WrapFunc(cpu.OR(cpu.registers.l)),
    /*0xB6*/ WrapFunc(cpu.OR(Address(cpu.registers.hl))),
    /*0xB7*/ WrapFunc(cpu.OR(cpu.registers.a)),
    /*0xB8*/ WrapFunc(cpu.CP(cpu.registers.b)),
    /*0xB9*/ WrapFunc(cpu.CP(cpu.registers.c)),
    /*0xBA*/ WrapFunc(cpu.CP(cpu.registers.d)),
    /*0xBB*/ WrapFunc(cpu.CP(cpu.registers.e)),
    /*0xBC*/ WrapFunc(cpu.CP(cpu.registers.h)),
    /*0xBD*/ WrapFunc(cpu.CP(cpu.registers.l)),
    /*0xBE*/ WrapFunc(cpu.CP(Address(cpu.registers.hl))),
    /*0xBF*/ WrapFunc(cpu.CP(cpu.registers.a)),

    /*0xC0*/ WrapFunc(cpu.RET(!cpu.registers.f.Z)),
    /*0xC1*/ WrapFunc(cpu.POP(cpu.registers.bc)),
    /*0xC2*/ WrapFunc(cpu.JP(!cpu.registers.f.Z)),
    /*0xC3*/ WrapFunc(cpu.JP()),
    /*0xC4*/ WrapFunc(cpu.CALL(!cpu.registers.f.Z)),
    /*0xC5*/ WrapFunc(cpu.PUSH(cpu.registers.bc)),
    /*0xC6*/ WrapFunc(cpu.AddData8()),
    /*0xC7*/ WrapFunc(cpu.RST(0x0)),
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