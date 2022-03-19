#include <cstring>

#include "NFS2/TRK/TrkFile.h"

using namespace LibOpenNFS::NFS2;

template <Platform platform>
void TrkFile<platform>::SerializeIn(std::istream &is)
{
    auto beginSeek = is.tellg();

    // Check we're in a valid TRK file
    Utils::SafeRead(is, header);

    // Header should contain TRAC
    if (strncmp(header, "TRAC", sizeof(header)) != 0)
        throw std::runtime_error{"Invalid TRK Header"};

    // Unknown header data
    Utils::SafeRead(is, unknownHeader);

    // Basic Track data
    Utils::SafeRead(is, nSuperBlocks);
    Utils::SafeRead(is, nBlocks);

    // Offsets of Superblocks in TRK file
    superBlockOffsets.resize(nSuperBlocks);
    Utils::SafeRead(is, superBlockOffsets.begin(), superBlockOffsets.end());

    // Reference coordinates for each block
    blockReferenceCoords.resize(nBlocks);
    Utils::SafeRead(is, blockReferenceCoords.begin(), blockReferenceCoords.end());

    // Go read the superblocks in
    for (uint32_t superBlockIdx = 0; superBlockIdx < nSuperBlocks; ++superBlockIdx)
    {
        // LOG(DEBUG) << "SuperBlock " << superBlockIdx + 1 << " of " << nSuperBlocks;
        //  Jump to the super block
        is.seekg(beginSeek + superBlockOffsets[superBlockIdx], std::ios_base::beg);
        is >> superBlocks.emplace_back();
    }
}

template class LibOpenNFS::NFS2::TrkFile<Platform::PS1>;
template class LibOpenNFS::NFS2::TrkFile<Platform::PC>;