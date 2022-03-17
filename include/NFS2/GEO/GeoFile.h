#pragma once

#include "Common/ISerializable.h"
#include "NFS2/Common.h"

namespace LibOpenNFS
{
    namespace NFS2
    {
        template <typename Platform>
        class GeoFile : ISerializable
        {
        public:
            static bool Load(const std::string &geoPath, GeoFile &geoFile);
            static void Save(const std::string &geoPath, GeoFile &geoFile);

        private:
            bool SerializeIn(std::istream &ifstream) override;
            void SerializeOut(std::ostream &ofstream) override;
        };
    } // namespace NFS2
} // namespace LibOpenNFS
