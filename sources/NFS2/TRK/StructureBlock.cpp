#include "NFS2/TRK/StructureBlock.h"
#include "Common/Utils.h"

using namespace LibOpenNFS::NFS2;

template <typename Platform>
StructureBlock<Platform>::StructureBlock(std::istream &ifstream)
{
    assert(this->SerializeIn(ifstream) && "Failed to serialize StructureBlock from file stream");
}

template <typename Platform>
bool StructureBlock<Platform>::SerializeIn(std::istream &ifstream)
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

    return true;
}

template <typename Platform>
void StructureBlock<Platform>::SerializeOut(std::ostream &ofstream)
{
    assert(false && "StructureBlock output serialization is not currently implemented");
}

template class LibOpenNFS::NFS2::StructureBlock<PS1>;
template class LibOpenNFS::NFS2::StructureBlock<PC>;