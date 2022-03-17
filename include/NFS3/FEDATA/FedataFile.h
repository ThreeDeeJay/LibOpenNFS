#pragma once

#include <cstdint>
#include <vector>

#include "Common/Utils.h"

namespace LibOpenNFS
{
    namespace NFS3
    {
        static constexpr uint32_t cColourTableOffset     = 0xA7;
        static constexpr uint32_t cMenuNameFilePosOffset = 0x37;

        class FedataFile : public IDeserializable
        {
        public:
            std::string menuName;
            std::vector<std::string> primaryColourNames;

        private:
            void SerializeIn(std::istream &ifstream) override;

            uint8_t m_nPriColours;
        };
    } // namespace NFS3
} // namespace LibOpenNFS
