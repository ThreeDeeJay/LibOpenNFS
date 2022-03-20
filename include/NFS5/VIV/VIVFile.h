#pragma once

#include <vector>

#include "Common/Utils.h"

namespace LibOpenNFS
{
    namespace NFS5
    {
        class BNKFile
        {
        public:
            std::string name;
            std::vector<char> data;
        };

        class VIVFile : public IDeserializable
        {
        public:
            // could be also .ASF file (zzzymus.viv)
            std::vector<BNKFile> files;

        protected:
            void SerializeIn(std::istream& ifstream) override;
        };
    } // namespace NFS5
} // namespace LibOpenNFS
