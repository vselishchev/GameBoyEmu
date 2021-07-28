#pragma once

#include "Utils.h"

namespace GameBoyEmu
{

struct MMU;

struct FlagRegister
{
    bool Z : 1; // Zero flag
    bool N : 1; // Add/sub flags (BCD)
    bool H : 1; // Half carry flag (BCD)
    bool C : 1; // Carry flag
    bool UNUSED : 4; // Unused, always zero
};

struct Registers
{
    union 
    {
        uint16 af;
        struct
        {
            FlagRegister f;
            uint8 a;
        };
    };
    union
    {
        uint16 bc;
        struct
        {
            uint8 c;
            uint8 b;
        };
    };
    union
    {
        uint16 de;
        struct
        {
            uint8 e;
            uint8 d;
        };
    };
    union
    {
        uint16 hl;
        struct
        {
            uint8 l;
            uint8 h;
        };
    };
    uint16 sp; // Stack pointer.
    uint16 pc; // Program counter.
};

class CPU
{
public:
    CPU(MMU& mmu);

    uint8  GetByteFromPC();
    uint16 GetWordFromPC();
    int8   GetSignedByteFromPC();
    int16  GetSignedWordFromPC();

private:
    void Tick(uint8 ticks = 1) {} // TODO

    // Flags modifiers
    void SetZeroFlag(uint8 value);
    void SetZeroFlag(uint16 value);
    void SetCarryFlag(uint8 oldValue, uint8 newValue);
    void SetCarryFlag(uint16 oldValue, uint16 newValue);

    // Opcodes
    void LoadRegData16(uint16& reg); // Load word data into the word register.
    void LoadRegData8(uint8& reg); // Load byte data into the byte register.
    void LoadAddrData8(Address address); // Load byte data into the addressed memory
    void LoadAddrSP(); // Load Stack Pointer into the address
    //void LoadAAddrReg(Address address); // Load value from address stored in register to register A.

    void CBOpcodeFunc();

    /// Instructions
    void NOP() { Tick();} // Takes CPU cycles.
    void None() {} // Called on invalid instruction.
    void STOP();
    void HALT();

    // Load
    void LD(uint8& to, uint8 from);
    void LD(uint16& to, uint16 from);
    void LD(uint8& to, Address address);
    void LD(uint16& to, Address address);
    void LD(Address address, uint8 from);
    void LD(Address address, uint16 from);

    // Increment/Decrement
    void INC(uint8& value);
    void INC(uint16& value);
    void INC(Address address);
    void DEC(uint8& value);
    void DEC(uint16& value);
    void DEC(Address address);

    // Add
    void ADD(uint8& to, uint8 from);
    void ADD(uint16& to, uint16 from);

    // Rotation
    void RLCA(); // Rotate A left
    void RLA(); // Rotate A left through carry
    void RRCA(); // Rotate A right
    void RRA(); // Rotate A right through carry

    // Jumps
    void JR(); // Relative jump to PC + signed data.
    void JR(bool flag); // Relative conditional jump to PC + signed data.

    void DAA(); // Decimal adjust A;
    void CPL(); // Coplement A (A = ~A)
    void SCF(); // Set Carry to 1;
    void CCF(); // Set Carry to carry xor 1;

private:
    MMU& memory;
    Registers registers;

    bool isHalted = false;
    // Keep them the last ones, so the other members are initialized before the tables.
    using OpcodeFunc = void(*)(CPU&);
    OpcodeFunc opcodesTable[0x100];
    OpcodeFunc CBOpcodeTable[0x100]; // This upcode table accessed if first byte of opcode was 0xCB.
};

}