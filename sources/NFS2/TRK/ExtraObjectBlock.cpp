#include <type_traits>

#include "NFS2/TRK/ExtraObjectBlock.h"
#include "Common/Utils.h"

using namespace LibOpenNFS;
using namespace LibOpenNFS::NFS2;

template <typename Platform>
void ExtraObjectBlock<Platform>::SerializeIn(std::istream &ifstream)
{
    // Read the header
    Utils::SafeRead(ifstream, recSize);
    Utils::SafeRead(ifstream, id);
    Utils::SafeRead(ifstream, nRecords);

    switch (id)
    {
    case 2: // First xblock always texture table (in COL)
        nTextures = nRecords;
        polyToQfsTexTable.resize(nTextures);
        Utils::SafeRead(ifstream, polyToQfsTexTable.begin(), polyToQfsTexTable.end());
        break;
    case 4:
        nNeighbours = nRecords;
        blockNeighbours.resize(nRecords);
        Utils::SafeRead(ifstream, blockNeighbours.begin(), blockNeighbours.end());
        break;
    case 5:
        polyTypes.resize(nRecords);
        Utils::SafeRead(ifstream, polyTypes.begin(), polyTypes.end());
        break;
    case 6:
        medianData.resize(nRecords);
        Utils::SafeRead(ifstream, medianData.begin(), medianData.end());
        break;
    case 7:
    case 18:
    case 19:
        nStructureReferences = nRecords;
        for (uint32_t structureRefIdx = 0; structureRefIdx < nStructureReferences; ++structureRefIdx)
        {
            StructureRefBlock structureReference;
            ifstream >> structureReference;
            structureReferences.push_back(std::move(structureReference));
        }
        break;
    case 8: // XBID 8 3D Structure data: This block is only present if nExtraBlocks != 2 (COL)
        nStructures = nRecords;
        for (uint32_t structureIdx = 0; structureIdx < nStructures; ++structureIdx)
        {
            StructureBlock<Platform> structure;
            ifstream >> structure;
            structures.push_back(std::move(structure));
        }
        break;
    case 9:
        nLanes = nRecords;
        laneData.resize(nRecords);
        Utils::SafeRead(ifstream, laneData.begin(), laneData.end());
        break;
    // case 10: // PS1 Specific id, Misc purpose
    // {
    // std::cout << "id 10 NStruct: " << nRecords << std::endl;
    // PS1::TRKBLOCK *ps1TrackBlock = ((PS1::TRKBLOCK *) trackblock);
    // ps1TrackBlock->nUnknownVerts = nRecords;
    // uint8_t xbidHeader[8];
    // trk.read((char *) xbidHeader, 8);
    // for (int i = 0; i < 8; ++i)
    // {
    //     std::cout << (int) xbidHeader[i] << std::endl;
    // }
    // // TODO: Likely these are not VERTS, and the act of adding the parent block center gives meaning where none is
    // present.ps1TrackBlock->unknownVerts = new PS1::VERT[nRecords];
    // for (uint32_t record_Idx = 0; record_Idx < nRecords; ++record_Idx)
    // {
    //     trk.read((char *) &ps1TrackBlock->unknownVerts[record_Idx], sizeof(PS1::VERT));
    // }
    // }
    // break;
    case 13:
        nVroad = nRecords;
        if constexpr (std::is_same_v<Platform, PS1>)
        {
            ps1VroadData.resize(nVroad);
            Utils::SafeRead(ifstream, ps1VroadData.begin(), ps1VroadData.end());
        }
        else if constexpr (std::is_same_v<Platform, PC>)
        {
            vroadData.resize(nVroad);
            Utils::SafeRead(ifstream, vroadData.begin(), vroadData.end());
        }
        else
        {
            throw;
        }
        break;
    case 15:
        nCollisionData = nRecords;
        collisionData.resize(nCollisionData);
        Utils::SafeRead(ifstream, collisionData.begin(), collisionData.end());
        break;
    default:
        throw;
        //    LOG(WARNING) << "Unknown XBID: " << id << " nRecords: " << nRecords << " RecSize: " << recSize;
        break;
    }
}

template class LibOpenNFS::NFS2::ExtraObjectBlock<PS1>;
template class LibOpenNFS::NFS2::ExtraObjectBlock<PC>;
