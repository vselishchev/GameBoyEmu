#pragma once

#include "Utils.h"

namespace GameBoyEmu
{

enum class Interrupts : uint8
{
    VBlank = 0x40,
    LCDStat = 0x48,
    Timer = 0x50,
    Serial = 0x58,
    Joypad = 0x60
}

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

    void   SetInterruptEnabled(uint8 IE); // Sets the IE register to specific combination, overwriting existing combination.
    void   AddInterruptEnabled(uint8 IE); // Adds provided IE bits to the IE register.
    void   RemoveInterruptEnabled(uint8 IE); // Removes specific set of bits from the IE register.

    void   SetInterruptFlag(uint8 IF); // Sets the IF register to specific combination, overwriting existing combination.
    void   AddInterruptFlag(uint8 IF); // Adds provided IF bits to the IF register.
    void   RemoveInterruptFlag(uint8 IF); // Removes specific set of bits from the IF register.

private:
    void Tick(uint8 ticks = 1) {} // TODO

    // Opcodes
    void LoadRegData16(uint16& reg); // Load word data into the word register.
    void LoadRegData8(uint8& reg); // Load byte data into the byte register.
    void LoadAddrData8(Address address); // Load byte data into the addressed memory
    void LoadAddrSP(); // Load Stack Pointer into the address
    void LoadAddrReg(uint8 reg); // Load value of the byte register in to the addressed memory.
    void LoadRegAddr(uint8& reg); // Load value from the addressed memory into the byte register.
    void LoadHLSPSignedData8(); // Load the value (SP + data8) into the register HL.
    void LoadSPHL();
    void AddData8();
    void AddSPSignedData();
    void SubData8();
    void AndData8();
    void OrData8();
    void AdcData8();
    void SbcData8();
    void XorData8();
    void CpData8();
    void WriteToIOPortAddress();
    void ReadFromIOPortAddress();

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

    // Add/Sub
    void ADD(uint8& to, uint8 from);
    void ADD(uint8& to, Address address);
    void ADD(uint16& to, uint16 from);
    void ADC(uint8& to, uint8 from);
    void ADC(uint8& to, Address address);
    void SUB(uint8 from);
    void SUB(Address address);
    void SBC(uint8 from);
    void SBC(Address address);

    // And/Or/Xor/Cp
    void AND(uint8 value);
    void AND(Address address);
    void OR(uint8 value);
    void OR(Address address);
    void XOR(uint8 value);
    void XOR(Address address);
    void CP(uint8 value);
    void CP(Address address);

    // Rotation
    void RLCA(); // Rotate A left
    void RLA(); // Rotate A left through carry
    void RRCA(); // Rotate A right
    void RRA(); // Rotate A right through carry

    // Jumps
    void JP(uint16 newPC);
    void JP(); // Jump to specified address.
    void JP(bool flag); // Conditional flag to specified address.
    void JR(); // Relative jump to PC + signed data.
    void JR(bool flag); // Relative conditional jump to PC + signed data.
    void CALL(Address address);
    void CALL(); 
    void CALL(bool flag);
    void RET();
    void RET(bool flag);
    void RST(Address address);

    // Stack ops
    void PUSH(uint16 value);
    void POP(uint16& to);

    // Interrupts
    void RETI();
    void EI();
    void DI();

    void DAA(); // Decimal adjust A;
    void CPL(); // Coplement A (A = ~A)
    void SCF(); // Set Carry to 1;
    void CCF(); // Set Carry to carry xor 1;

private:
    MMU&      memory;
    Registers registers;

    bool      isHalted = false;
    uint8     ime = 0; // interrupt master enable flag.
    uint8     interruptsEnable = 0;
    uint8     interruptsFlag = 0;

    // Keep them the last ones, so the other members are initialized before the tables.
    using OpcodeFunc = void(*)(CPU&);
    OpcodeFunc opcodesTable[0x100];
    OpcodeFunc CBOpcodeTable[0x100]; // This upcode table accessed if first byte of opcode was 0xCB.
};

}