#pragma once

#include "Common/Utils.h"

#include "NFS3/FRD/TrkBlock.h"
#include "NFS3/FRD/PolyBlock.h"
#include "NFS3/FRD/ExtraObjectBlock.h"
#include "NFS3/FRD/TexBlock.h"

namespace LibOpenNFS
{
    namespace NFS3
    {
        static constexpr uint8_t cHeaderLength = 28;

        class FrdFile : public ISerializable, public IDeserializable
        {
        public:
            void FrdFile::MergeFRD(const FrdFile &frdFile);

            // Raw File data
            char header[cHeaderLength];
            uint32_t nBlocks;
            uint32_t nTextures;
            
            std::vector<TrkBlock> trackBlocks;
            std::vector<PolyBlock> polygonBlocks;
            std::vector<ExtraObjectBlock> extraObjectBlocks;
            std::vector<TexBlock> textureBlocks;

        private:
            void SerializeIn(std::istream &ifstream) override;
            void SerializeOut(std::ostream &ofstream) const override;
        };
    } // namespace NFS3
} // namespace LibOpenNFS
