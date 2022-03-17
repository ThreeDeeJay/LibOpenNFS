#pragma once

#include "../../Common/ISerializable.h"

namespace LibOpenNFS
{
    namespace NFS3
    {
        class SpeedsFile : ISerializable
        {
        public:
            SpeedsFile() = default;
            static bool Load(const std::string &speedBinPath, SpeedsFile &speedFile);
            static void Save(const std::string &speedBinPath, SpeedsFile &speedFile);
            static void SaveCSV(const std::string &speedsCsvPath, SpeedsFile &speedFile);

            std::vector<uint8_t> speeds;

        private:
            bool SerializeIn(std::istream &ifstream) override;
            void SerializeOut(std::ostream &ofstream) override;

            uint16_t m_uFileSize = 0;
        };
    } // namespace NFS3
} // namespace LibOpenNFS
