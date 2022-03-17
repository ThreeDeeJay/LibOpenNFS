#pragma once

#include "../../Common/ISerializable.h"

namespace LibOpenNFS
{
    namespace NFS3
    {
        class TexBlock : public ISerializable
        {
        public:
            TexBlock() = default;
            explicit TexBlock(std::istream &frd);
            void SerializeOut(std::ostream &ifstream) override;

            uint16_t width, height;
            uint32_t unknown1; // Blending related, hometown covered bridges godrays `
            float corners[8];  // 4x planar coordinates == tiling?
            uint32_t unknown2;
            bool isLane;       // 1 if not a real texture (lane), 0 usually
            uint16_t qfsIndex; // index in QFS file

        private:
            bool SerializeIn(std::istream &ofstream) override;
        };
    } // namespace NFS3
} // namespace LibOpenNFS
