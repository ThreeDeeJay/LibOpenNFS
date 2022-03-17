#pragma once

#include <fstream>
#include <cstdint>
#include <array>

constexpr uint32_t ONFS_SIGNATURE                     = 0x15B001C0;
constexpr std::array<uint8_t, 6> quadToTriVertNumbers = {0, 1, 2, 0, 2, 3};

class ISerializable
{
protected:
    virtual bool SerializeIn(std::istream &ifstream)  = 0;
    virtual void SerializeOut(std::ostream &ofstream) = 0;
};
