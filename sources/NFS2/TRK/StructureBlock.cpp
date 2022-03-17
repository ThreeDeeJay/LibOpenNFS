#include "NFS2/TRK/StructureBlock.h"

using namespace LibOpenNFS::NFS2;

template <Platform platform>
void StructureBlock<platform>::SerializeIn(std::istream &ifstream)
{
    std::streamoff padCheck = ifstream.tellg();

    Utils::SafeRead(ifstream, recSize);
    Utils::SafeRead(ifstream, nVerts);
    Utils::SafeRead(ifstream, nPoly);

    vertexTable.resize(nVerts);
    Utils::SafeRead(ifstream, vertexTable.begin(), vertexTable.end());

    polygonTable.resize(nPoly);
    Utils::SafeRead(ifstream, polygonTable.begin(), polygonTable.end());

    ifstream.seekg(recSize - (ifstream.tellg() - padCheck), std::ios_base::cur); // Eat possible padding
}

template class LibOpenNFS::NFS2::StructureBlock<Platform::PS1>;
template class LibOpenNFS::NFS2::StructureBlock<Platform::PC>;