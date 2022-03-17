#pragma once

#include <cstdint>
#include <vector>

#include "Common/Utils.h"

namespace LibOpenNFS
{
    namespace NFS3
    {
        struct Header
        {
            char fntfChk[4];
            uint32_t fileSize;
            uint16_t version;
            uint16_t numChars;
            uint32_t unknown;
            uint16_t zeroPad;
            uint16_t unknown2;
            uint64_t num20Check;
            uint32_t fontMapOffset;
        };

        struct CharTableEntry
        {
            uint16_t asciiCode;
            uint8_t width;
            uint8_t height;
            uint8_t horizontalScroll;
            uint8_t unknown[2];
            uint8_t zeroPad;
            uint8_t rightPadding;
            uint8_t leftPadding;
            uint8_t topPadding;
        };

        class FfnFile : public IDeserializable
        {
        public:
            Header header;

            std::vector<CharTableEntry> characters;

        private:
            void SerializeIn(std::istream &ifstream) override;
        };
    } // namespace NFS3
} // namespace LibOpenNFS
