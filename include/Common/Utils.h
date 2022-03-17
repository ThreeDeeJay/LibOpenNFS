#pragma once

#include <cstdlib>
#include <iterator>
#include <istream>
#include <ostream>
#include <array>
#include <cstdint>

namespace LibOpenNFS
{
    namespace Utils
    {
        constexpr std::array<uint8_t, 6> quadToTriVertNumbers{0, 1, 2, 0, 2, 3};

        template <typename T>
        void SafeRead(std::istream& file, T* ptr, size_t size)
        {
            if (file.read(reinterpret_cast<char*>(ptr), size).gcount() != size)
                throw std::runtime_error{"Failed to safe read from stream"};
        }

        template <typename T>
        void SafeRead(std::istream& file, T& ref)
        {
            SafeRead(file, &ref, sizeof(T));
        }

        template <typename T, std::size_t N>
        void SafeRead(std::istream& file, T (&arr)[N])
        {
            SafeRead(file, arr, sizeof(T) * N);
        }

        template <typename InputIt>
        void SafeRead(std::istream& file, InputIt first, InputIt last)
        {
            SafeRead(file, &(*first), std::distance(first, last) * sizeof(typename InputIt::value_type));
        }
    } // namespace Utils
} // namespace LibOpenNFS

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