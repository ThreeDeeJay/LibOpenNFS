#include <cstring>

#include "NFS2/TRK/TrkFile.h"

using namespace LibOpenNFS::NFS2;

template <Platform platform>
void TrkFile<platform>::SerializeIn(std::istream &ifstream)
{
    // Check we're in a valid TRK file
    Utils::SafeRead(ifstream, header);

    // Header should contain TRAC
    if (strncmp(header, "TRAC", sizeof(header)) != 0)
        throw std::runtime_error{"Invalid TRK Header"};

    // Unknown header data
    Utils::SafeRead(ifstream, unknownHeader);

    // Basic Track data
    Utils::SafeRead(ifstream, nSuperBlocks);
    Utils::SafeRead(ifstream, nBlocks);

    // Offsets of Superblocks in TRK file
    superBlockOffsets.resize(nSuperBlocks);
    Utils::SafeRead(ifstream, superBlockOffsets.begin(), superBlockOffsets.end());

    // Reference coordinates for each block
    blockReferenceCoords.resize(nBlocks);
    Utils::SafeRead(ifstream, blockReferenceCoords.begin(), blockReferenceCoords.end());

    // Go read the superblocks in
    for (uint32_t superBlockIdx = 0; superBlockIdx < nSuperBlocks; ++superBlockIdx)
    {
        // LOG(DEBUG) << "SuperBlock " << superBlockIdx + 1 << " of " << nSuperBlocks;
        //  Jump to the super block
        ifstream.seekg(superBlockOffsets[superBlockIdx], std::ios_base::beg);

        SuperBlock<platform> superBlock;
        ifstream >> superBlock;
        superBlocks.push_back(std::move(superBlock));
    }
}

template class LibOpenNFS::NFS2::TrkFile<Platform::PS1>;
template class LibOpenNFS::NFS2::TrkFile<Platform::PC>;