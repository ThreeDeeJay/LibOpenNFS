#pragma once

#include <cstdint>
#include <vector>

#include "Common/Utils.h"

namespace LibOpenNFS
{
    namespace NFS3
    {
        class SpeedsFile : public IDeserializable, public ISerializable
        {
            std::vector<uint8_t> speeds;

        private:
            void SerializeIn(std::istream &ifstream) override;
            void SerializeOut(std::ostream& ofstream) const override;

            uint16_t m_uFileSize = 0;
        };
    } // namespace NFS3
} // namespace LibOpenNFS
