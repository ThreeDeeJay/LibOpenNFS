#pragma once

#include "../../Common/ISerializable.h"

namespace LibOpenNFS
{
    namespace NFS3
    {
        static const uint32_t COLOUR_TABLE_OFFSET      = 0xA7;
        static const uint32_t MENU_NAME_FILEPOS_OFFSET = 0x37;

        class FedataFile : ISerializable
        {
        public:
            FedataFile() = default;

            static bool Load(const std::string &fedataPath, FedataFile &fedataFile, uint8_t nPriColours);
            static void Save(const std::string &fedataPath, FedataFile &fedataFile);

            std::string menuName;
            std::vector<std::string> primaryColourNames;

        private:
            bool SerializeIn(std::istream &ifstream) override;
            void SerializeOut(std::ostream &ofstream) override;

            uint8_t m_nPriColours;
        };
    } // namespace NFS3
} // namespace LibOpenNFS
