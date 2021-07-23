#include "CPU.h"

#include "MMU.h"

namespace GameBoyEmu
{

void CPU::LoadRegData16(uint16& reg)
{
    const uint16 data = memory.ReadWord(registers.pc); // read immediate word
    registers.pc += 2;
    
    LD(reg, data);
}

void CPU::LoadAddrRegA(Address address)
{
    ++registers.pc;
    LD(address, registers.a);
}

void CPU::CBOpcodeFunc()
{ 
    const uint8 secondByte = memory.ReadByte(registers.pc++);
    //(this->*CBOpcodeTable[secondByte])();
}

void CPU::LD(uint8& to, uint8 from)
{

}

void CPU::LD(uint16& to, uint16 from)
{
    to = from;
    Tick(3);
}

void CPU::LD(uint8& to, Address address)
{

}

void CPU::LD(uint16& to, Address address)
{
}

void CPU::LD(Address address, uint8 from)
{
    memory.WriteByte(address.GetValue(), from);
    Tick(2); // Lowest tick for this set of operations is 2. Add extra in calling functions if needed.
}

void CPU::LD(Address address, uint16 from)
{

}

void CPU::INC(uint8& value)
{
    ++value;
    Tick();

    // Continue here
}

void CPU::INC(uint16& value)
{
    ++value;
    Tick(2);
}

void CPU::INC(Address address)
{

}

}