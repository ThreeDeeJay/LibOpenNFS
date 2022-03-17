#pragma once

#include "../../Common/ISerializable.h"

#include "TrkBlock.h"
#include "PolyBlock.h"
#include "ExtraObjectBlock.h"
#include "TexBlock.h"

namespace LibOpenNFS
{
    namespace NFS3
    {
        static const uint8_t HEADER_LENGTH = 28;

        class FrdFile : ISerializable
        {
        public:
            FrdFile() = default;
            static bool Load(const std::string &frdPath, FrdFile &frdFile);
            static void Save(const std::string &frdPath, FrdFile &frdFile);
            static void MergeFRD(const std::string &frdPath, FrdFile &frdFileA, FrdFile &frdFileB);

            // Raw File data
            char header[HEADER_LENGTH];
            uint32_t nBlocks;
            uint32_t nTextures;
            NFSVer version;
            std::vector<TrkBlock> trackBlocks;
            std::vector<PolyBlock> polygonBlocks;
            std::vector<ExtraObjectBlock> extraObjectBlocks;
            std::vector<TexBlock> textureBlocks;

        private:
            bool SerializeIn(std::istream &ifstream) override;
            void SerializeOut(std::ostream &ofstream) override;
        };
    } // namespace NFS3
} // namespace LibOpenNFS
