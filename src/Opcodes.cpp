#include "CPU.h"

#include "MMU.h"

namespace GameBoyEmu
{

void CPU::STOP()
{
    ++registers.pc;
    Tick();
}

void CPU::HALT()
{
    isHalted = true;
    Tick();
}

void CPU::LoadRegData16(uint16& reg)
{
    const uint16 data = GetWordFromPC();
    LD(reg, data);
}

void CPU::LoadRegData8(uint8& reg)
{
    const uint8 data = GetByteFromPC();
    LD(reg, data);
    Tick(); // 2 Ticks in total.
}

void CPU::LoadAddrData8(Address address)
{
    const uint8 data = GetByteFromPC();
    LD(address, data);
    Tick(); // 3 ticks in total.
}

void CPU::LoadAddrSP()
{
    const Address addr = memory.ReadWord(registers.pc);
    registers.pc += 2;

    LD(addr, registers.sp);
    Tick(); // 4 Ticks in total.
}

void CPU::CBOpcodeFunc()
{ 
    const uint8 secondByte = memory.ReadByte(registers.pc++);
    //(this->*CBOpcodeTable[secondByte])();
}

void CPU::LD(uint8& to, uint8 from)
{
    to = from;
    Tick();
}

void CPU::LD(uint16& to, uint16 from)
{
    to = from;
    Tick(3);
}

void CPU::LD(uint8& to, Address address)
{
    to = memory.ReadByte(address);
    Tick(2);
}

void CPU::LD(uint16& to, Address address)
{
}

void CPU::LD(Address address, uint8 from)
{
    memory.WriteByte(address, from);
    Tick(2); // Lowest tick for this set of operations is 2. Add extra in calling functions if needed.
}

void CPU::LD(Address address, uint16 from)
{
    memory.WriteWord(address, from);
    Tick(3);
}

void CPU::INC(uint8& value)
{
    const bool isBitSetBefore = ISBITSET(value, 3); // Test forth bit for half carry.
    ++value;
    const bool isBitSetAfter = ISBITSET(value, 3);
    Tick();

    registers.f.Z = value == 0;
    registers.f.N = 0;
    registers.f.H = isBitSetBefore && (isBitSetAfter != isBitSetBefore);
}

void CPU::INC(uint16& value)
{
    ++value;
    Tick(2);
}

void CPU::INC(Address address)
{
    uint8 value = memory.ReadByte(address);
    INC(value);
    memory.WriteByte(address, value);
    Tick(2); // 3 ticks in total.
}

void CPU::DEC(uint8& value)
{
    const uint8 initValue = value--;
    Tick();

    registers.f.Z = value == 0;
    registers.f.N = 1;
    registers.f.H = ((value ^ 0x1 ^ initValue) & 0xF0) == 0x10;
}

void CPU::DEC(uint16& value)
{
    --value;
    Tick(2);
}

void CPU::DEC(Address address)
{
    uint8 value = memory.ReadByte(address);
    DEC(value);
    memory.WriteByte(address, value);
    Tick(2); // 3 ticks in total.
}

void CPU::ADD(uint8& to, uint8 from)
{

}

void CPU::ADD(uint16& to, uint16 from)
{
    const bool hasHalfCarry = ((to & 0x0FFF) + (from & 0x0FFF)) > 0x0FFF;
    const bool lastBitBefore = ISBITSET(to, 15);
    to += from;
    const bool lastBitAfter = ISBITSET(to, 15);

    registers.f.N = 0;
    registers.f.H = hasHalfCarry;
    registers.f.C = lastBitBefore && (lastBitBefore != lastBitAfter);
    Tick(2);
}

void CPU::RLCA()
{
    registers.f.C = registers.a & 0x80; // If 7th bit is 1, set carry flag to true.
    registers.a = (registers.a << 1) | (registers.a >> 7);
    Tick();
    registers.f.Z = 0;
    registers.f.N = 0;
    registers.f.H = 0;
}

void CPU::RLA()
{
    const uint8 newCarry = registers.a & 0x80;
    registers.a = (registers.a << 1) | registers.f.C;
    Tick();
    registers.f.Z = 0;
    registers.f.N = 0;
    registers.f.H = 0;
    registers.f.C = newCarry;
}

void CPU::RRCA()
{
    registers.f.C = registers.a & 0x1; // If 0th bit is 1, set carry to true.
    registers.a = (registers.a >> 1) | (registers.a << 7);
    registers.f.Z = 0;
    registers.f.N = 0;
    registers.f.H = 0;
    Tick();
}

void CPU::RRA()
{
    const uint8 newCarry = registers.a & 0x1;
    registers.a = (registers.a >> 1) | (registers.f.C << 7);
    Tick();
    registers.f.Z = 0;
    registers.f.N = 0;
    registers.f.H = 0;
    registers.f.C = newCarry;
}

void CPU::JR()
{
    int8 relativeIndex = GetSignedByteFromPC();
    registers.pc += relativeIndex;
    Tick(3);
}

void CPU::JR(bool flag)
{
    if (flag)
    {
        JR();
    }
    else
    {
        ++registers.pc;
        Tick(2);
    }
}


// Stole it from here: https://github.com/jgilchrist/gbemu/blob/master/src/cpu/opcodes.cc
// Still need to investigate if it's doing right thing.
void CPU::DAA()
{
    uint8 reg = registers.a;

    uint16 correction = registers.f.C
        ? 0x60
        : 0x00;

    if (registers.f.H || (!registers.f.N && ((reg & 0x0F) > 9))) {
        correction |= 0x06;
    }

    if (registers.f.C || (!registers.f.N && (reg > 0x99))) {
        correction |= 0x60;
    }

    if (registers.f.N) {
        reg = static_cast<uint8>(reg - correction);
    } else {
        reg = static_cast<uint8>(reg + correction);
    }

    if (((correction << 2) & 0x100) != 0) {
        registers.f.C = true;
    }

    registers.f.H = false;
    registers.f.Z = reg == 0;

    registers.a = reg;
    Tick();
}

void CPU::CPL()
{
    registers.a = ~registers.a;
    Tick();

    registers.f.N = 1;
    registers.f.H = 1;
}

void CPU::SCF()
{
    registers.f.C = 1;
    Tick();
    registers.f.N = 0;
    registers.f.H = 0;
}

void CPU::CCF()
{
    registers.f.C ^= 1;
    Tick();
    registers.f.N = 0;
    registers.f.H = 0;
}

}