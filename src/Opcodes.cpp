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

void CPU::AddData8()
{
    const uint8 data = GetByteFromPC();
    ADD(registers.a, data);
    Tick(); // 2 Ticks in total.
}

void CPU::SubData8()
{
    const uint8 data = GetByteFromPC();
    SUB(data);
    Tick(); // 2 Ticks in total.
}

void CPU::AndData8()
{
    const uint8 data = GetByteFromPC();
    AND(data);
    Tick(); // 2 Ticks in total.
}

void CPU::OrData8()
{
    const uint8 data = GetByteFromPC();
    OR(data);
    Tick(); // 2 Ticks in total.
}

void CPU::AdcData8()
{
    const uint8 data = GetByteFromPC();
    ADC(registers.a, data);
    Tick(); // 2 Ticks in total.
}

void CPU::SbcData8()
{
    const uint8 data = GetByteFromPC();
    SBC(data);
    Tick(); // 2 Ticks in total.
}

void CPU::XorData8()
{
    const uint8 data = GetByteFromPC();
    XOR(data);
    Tick(); // 2 Ticks in total.
}

void CPU::CpData8()
{
    const uint8 data = GetByteFromPC();
    CP(data);
    Tick(); // 2 Ticks in total.
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
    const bool hasHalfCarry = ((to & 0x0F) + (from & 0x0F)) > 0x0F;
    const bool hasCarry = ((to & 0xFF) + (from & 0xFF)) > 0xFF;
    to += from;

    registers.f.Z = to == 0;
    registers.f.N = 0;
    registers.f.H = hasHalfCarry;
    registers.f.C = hasCarry;
    Tick();
}

void CPU::ADD(uint8& to, Address address)
{
    const uint8 data = memory.ReadByte(address);
    ADD(to, data);
    Tick(); // 2 ticks in total.
}

void CPU::ADD(uint16& to, uint16 from)
{
    const bool hasHalfCarry = ((to & 0x0FFF) + (from & 0x0FFF)) > 0x0FFF;
    const bool hasCarry = ((to & 0xFFFF) + (from & 0xFFFF)) > 0xFFFF;
    to += from;

    registers.f.N = 0;
    registers.f.H = hasHalfCarry;
    registers.f.C = hasCarry;
    Tick(2);
}

void CPU::ADC(uint8& to, uint8 from)
{
    ADD(to, from + registers.f.C);
}

void CPU::ADC(uint8& to, Address address)
{
    const uint8 data = memory.ReadByte(address);
    ADD(to, data + registers.f.C);
    Tick(); // 2 ticks in total.
}

void CPU::SUB(uint8 from)
{
    const uint8 oldValue = registers.a;
    registers.a -= from;
    registers.f.Z = registers.a == 0;
    registers.f.N = 1;
    registers.f.H = (oldValue & 0x0F) < (from & 0x0F);
    registers.f.C = oldValue < from;
    Tick();
}

void CPU::SUB(Address address)
{
    const uint8 data = memory.ReadByte(address);
    SUB(data);
    Tick(); // 2 ticks in total.
}

void CPU::SBC(uint8 from)
{
    SUB(from - registers.f.C);
}

void CPU::SBC(Address address)
{
    const uint8 data = memory.ReadByte(address);
    SUB(data - registers.f.C);
    Tick(); // 2 ticks in total.
}

void CPU::AND(uint8 value)
{
    registers.a &= value;
    registers.f.Z = registers.a == 0;
    registers.f.N = 0;
    registers.f.H = 1;
    registers.f.C = 0;
    Tick();
}

void CPU::AND(Address address)
{
    const uint8 data = memory.ReadByte(address);
    AND(data);
    Tick(); // 2 ticks in total.
}

void CPU::OR(uint8 value)
{
    registers.a |= value;
    registers.f.Z = registers.a == 0;
    registers.f.N = 0;
    registers.f.H = 0;
    registers.f.C = 0;
    Tick();
}

void CPU::OR(Address address)
{
    const uint8 data = memory.ReadByte(address);
    OR(data);
    Tick(); // 2 ticks in total.
}

void CPU::XOR(uint8 value)
{
    registers.a ^= value;
    registers.f.Z = registers.a == 0;
    registers.f.N = 0;
    registers.f.H = 0;
    registers.f.C = 0;
    Tick();
}

void CPU::XOR(Address address)
{
    const uint8 data = memory.ReadByte(address);
    XOR(data);
    Tick(); // 2 ticks in total.
}

void CPU::CP(uint8 value)
{
    registers.f.Z = registers.a == value;
    registers.f.N = 1;
    registers.f.H = (registers.a & 0x0F) < (value & 0x0F);
    registers.f.C = registers.a < value;
    Tick();
}

void CPU::CP(Address address)
{
    const uint8 data = memory.ReadByte(address);
    CP(data);
    Tick(); // 2 ticks in total.
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

void CPU::JP()
{
    const uint16 address = GetWordFromPC();
    registers.pc = address;
    Tick(4);
}

void CPU::JP(bool flag)
{
    if (flag)
    {
        JP();

    }
    else
    {
        registers.pc += 2;
        Tick(3);
    }
}

void CPU::JR()
{
    const int8 relativeIndex = GetSignedByteFromPC();
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

void CPU::CALL(Address address)
{
    PUSH(registers.pc);
    registers.pc = address;
    Tick(2);
}

void CPU::CALL()
{
    const uint16 newPC = GetWordFromPC();
    CALL(newPC);
    Tick(2);
}

void CPU::CALL(bool flag)
{
    if (flag)
    {
        CALL();
    }
    else
    {
        registers.pc += 2;
        Tick(3);
    }
}

void CPU::RET()
{
    POP(registers.pc);
    Tick(1); // 4 ticks in total.
}

void CPU::RET(bool flag)
{
    if (flag)
    {
        RET();
        Tick(); // 5 ticks in total.
    }
    else
    {
        Tick(2);
    }
}

void CPU::RST(Address address)
{
    PUSH(registers.pc);
    registers.pc = address;
}

void CPU::PUSH(uint16 value)
{
    registers.sp -= 2;
    memory.WriteWord(registers.sp, value);
    Tick(4);
}

void CPU::POP(uint16& to)
{
    to = memory.ReadWord(registers.sp);
    registers.sp += 2;
    Tick(3);
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