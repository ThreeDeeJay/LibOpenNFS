#pragma once

#include <cstdlib> 
#include <iterator>
#include <fstream>

namespace LibOpenNFS
{
    namespace Utils
    {
        template <typename T>
        void SafeRead(std::istream &file, T *ptr, size_t size)
        {
            if (file.read(reinterpret_cast<char *>(ptr), size).gcount() != size)
                throw std::runtime_error{"Failed to safe read from stream"};
        }

        template <typename T>
        void SafeRead(std::istream &file, T &ref)
        {
            SafeRead(file, &ref, sizeof(T));
        }

        template <typename T, std::size_t N>
        void SafeRead(std::istream &file, T (&arr)[N])
        {
            SafeRead(file, arr, sizeof(T) * N);
        }

        template <typename InputIt>
        void SafeRead(std::istream &file, InputIt first, InputIt last)
        {
            SafeRead(file, &(*first), std::distance(first, last) * sizeof(typename InputIt::value_type));
        }
    } // namespace Utils
} // namespace LibOpenNFS