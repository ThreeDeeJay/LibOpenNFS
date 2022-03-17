#pragma once

#include <fstream>
#include <cstdint>
#include <array>
#include <ostream>

constexpr uint32_t ONFS_SIGNATURE                     = 0x15B001C0;
constexpr std::array<uint8_t, 6> quadToTriVertNumbers = {0, 1, 2, 0, 2, 3};

class ISerializable
{
public:
    friend std::ostream& operator<<(std::ostream& os, const ISerializable& ref);

protected:
    virtual void SerializeOut(std::ostream& ofstream) const = 0;
};

std::ostream& operator<<(std::ostream& os, const ISerializable& ref);

class IDeserializable
{
public:
    friend std::istream& operator>>(std::istream& is, IDeserializable& ref);

protected:
    virtual void SerializeIn(std::istream& ifstream) = 0;
};

std::istream& operator>>(std::istream& is, IDeserializable& ref);