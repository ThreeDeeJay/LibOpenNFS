#include "NFS3/FRD/FrdFile.h"

#include <fstream>

using namespace LibOpenNFS::NFS3;

void FrdFile::MergeFRD(const FrdFile &frdFile)
{
    // Mergearooney
    // TODO: Of course it couldn't be this simple :(
    nBlocks += frdFile.nBlocks;
    trackBlocks.insert(trackBlocks.end(), frdFile.trackBlocks.begin(), frdFile.trackBlocks.end());
    polygonBlocks.insert(polygonBlocks.end(), frdFile.polygonBlocks.begin(), frdFile.polygonBlocks.end());
    extraObjectBlocks.insert(extraObjectBlocks.end(), frdFile.extraObjectBlocks.begin(), frdFile.extraObjectBlocks.end());
}

void FrdFile::SerializeIn(std::istream &ifstream)
{
    Utils::SafeRead(ifstream, header);
    Utils::SafeRead(ifstream, nBlocks);
    ++nBlocks;

    if (nBlocks < 1 || nBlocks > 500)
        throw;

    // Detect NFS3 or NFSHS
    int32_t hsMagic = 0;
    Utils::SafeRead(ifstream, hsMagic);

    //if ((hsMagic < 0) || (hsMagic > 5000))
    //    version = NFSVer::NFS_3;
    //else if (((hsMagic + 7) / 8) == nBlocks)
    //    version = NFSVer::NFS_4;
    //else
        // Unknown file type
    //    throw;

    // Back up a little, as this sizeof(int32_t) into a trackblock that we're about to deserialize
    ifstream.seekg(-4, std::ios_base::cur);

    // Track Data
    trackBlocks.reserve(nBlocks);
    for (uint32_t blockIdx = 0; blockIdx < nBlocks; ++blockIdx)
        ifstream >> trackBlocks.emplace_back();
    
    // Geometry
    polygonBlocks.reserve(nBlocks);
    for (uint32_t blockIdx = 0; blockIdx < nBlocks; ++blockIdx)
        ifstream >> polygonBlocks.emplace_back();

    // Extra Track Geometry
    extraObjectBlocks.reserve((4 * nBlocks) + 1);
    for (uint32_t blockIdx = 0; blockIdx <= 4 * nBlocks; ++blockIdx)
        ifstream >> extraObjectBlocks.emplace_back();

    // Texture Table
    Utils::SafeRead(ifstream, nTextures);

    textureBlocks.reserve(nTextures);
    for (uint32_t tex_Idx = 0; tex_Idx < nTextures; ++tex_Idx)
        ifstream >> textureBlocks.emplace_back();
}

void FrdFile::SerializeOut(std::ostream &ofstream) const
{
    // Write FRD Header
    ofstream.write((char *) &header, sizeof(header));
    uint32_t nBlocksHeader = nBlocks - 1;
    ofstream.write((char *) &nBlocksHeader, sizeof(nBlocksHeader));

    // Track Data
    for (const auto &trackBlock : trackBlocks)
        ofstream << trackBlock;

    // Geometry
    for (const auto &polyBlock : polygonBlocks)
        ofstream << polyBlock;
    
    // Extra Track Geometry
    for (const auto &extraObjectBlock : extraObjectBlocks)
        ofstream << extraObjectBlock;

    // Texture Table
    ofstream.write((char *) &nTextures, sizeof(uint32_t));
    for (const auto &textureBlock : textureBlocks)
        ofstream << textureBlock;

    // ofstream.write((char *) &ONFS_SIGNATURE, sizeof(uint32_t));
}
