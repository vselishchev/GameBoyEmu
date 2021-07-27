#pragma once

namespace GameBoyEmu
{

using uint8 = unsigned char;
using uint16 = unsigned short;
using int8 = signed char;
using int16 = signed short;

constexpr uint16 BankSize = 0x4000;
constexpr uint16 VideoRAMSize = 0x2000;
constexpr uint16 ExternalRAMSize = 0x2000;
constexpr uint16 WorkRAMSize = 0x2000;
constexpr uint16 ShadowWorkRAMSize = 0x1E00;
constexpr uint8 OAMSize = 0xA0;
constexpr uint8 BlobSize = 0x60;
constexpr uint8 IORegistersSize = 0x80;
constexpr uint8 HighRAMSize = 0x7F;

class Address
{
public:
    Address(uint16 addr) : address(addr) {}

    uint16 GetValue() const { return address; }
    operator uint16() { return address; }

private:
    uint16 address;
};

#define ISBITSET(value, bit) ((value >> bit) & 0x1)

}