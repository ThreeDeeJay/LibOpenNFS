#pragma once

#include "Common/ISerializable.h"
#include "NFS2/Common.h"

namespace LibOpenNFS
{
    namespace NFS2
    {
        template <typename Platform>
        class GeoFile : public IDeserializable
        {
        protected:
            void SerializeIn(std::istream &ifstream) override;
        };
    } // namespace NFS2
} // namespace LibOpenNFS