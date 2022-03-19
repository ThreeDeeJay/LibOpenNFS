#include "NFS2/TRK/StructureBlock.h"

using namespace LibOpenNFS::NFS2;

template <Platform platform>
void StructureBlock<platform>::SerializeIn(std::istream &is)
{
    auto padCheck = is.tellg();

    Utils::SafeRead(is, recSize);
    Utils::SafeRead(is, nVerts);
    Utils::SafeRead(is, nPoly);

    vertexTable.resize(nVerts);
    Utils::SafeRead(is, vertexTable.begin(), vertexTable.end());

    polygonTable.resize(nPoly);
    Utils::SafeRead(is, polygonTable.begin(), polygonTable.end());

    is.seekg(recSize - (is.tellg() - padCheck), std::ios_base::cur); // Eat possible padding
}

template class LibOpenNFS::NFS2::StructureBlock<Platform::PS1>;
template class LibOpenNFS::NFS2::StructureBlock<Platform::PC>;