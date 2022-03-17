#include "NFS3/FCE/FceFile.h"

using namespace LibOpenNFS::NFS3;

void FceFile::SerializeIn(std::istream &ifstream)
{
    Utils::SafeRead(ifstream, unknown);
    Utils::SafeRead(ifstream, nTriangles);
    Utils::SafeRead(ifstream, nVertices);
    Utils::SafeRead(ifstream, nArts);
    Utils::SafeRead(ifstream, vertTblOffset);
    Utils::SafeRead(ifstream, normTblOffset);
    Utils::SafeRead(ifstream, triTblOffset);
    Utils::SafeRead(ifstream, reserve1Offset);
    Utils::SafeRead(ifstream, reserve2Offset);
    Utils::SafeRead(ifstream, reserve3Offset);
    Utils::SafeRead(ifstream, modelHalfSize);
    Utils::SafeRead(ifstream, nDummies);
    Utils::SafeRead(ifstream, dummyCoords);
    Utils::SafeRead(ifstream, nParts);
    Utils::SafeRead(ifstream, partCoords);
    Utils::SafeRead(ifstream, partFirstVertIndices);
    Utils::SafeRead(ifstream, partNumVertices);
    Utils::SafeRead(ifstream, partFirstTriIndices);
    Utils::SafeRead(ifstream, partNumTriangles);
    Utils::SafeRead(ifstream, nPriColours);
    Utils::SafeRead(ifstream, primaryColours);
    Utils::SafeRead(ifstream, nSecColours);
    Utils::SafeRead(ifstream, secondaryColours);
    Utils::SafeRead(ifstream, dummyNames);
    Utils::SafeRead(ifstream, partNames);
    Utils::SafeRead(ifstream, unknownTable);

    carParts.resize(nParts);

    for (uint32_t partIdx = 0; partIdx < nParts; ++partIdx)
    {
        carParts[partIdx].vertices.resize(partNumVertices[partIdx]);
        ifstream.seekg(0x1F04 + vertTblOffset + (partFirstVertIndices[partIdx] * sizeof(glm::vec3)), std::ios_base::beg);
        Utils::SafeRead(ifstream, carParts[partIdx].vertices.begin(), carParts[partIdx].vertices.end());

        carParts[partIdx].normals.resize(partNumVertices[partIdx]);
        ifstream.seekg(0x1F04 + normTblOffset + (partFirstVertIndices[partIdx] * sizeof(glm::vec3)), std::ios_base::beg);
        Utils::SafeRead(ifstream, carParts[partIdx].normals.begin(), carParts[partIdx].normals.end());

        carParts[partIdx].triangles.resize(partNumTriangles[partIdx]);
        ifstream.seekg(0x1F04 + triTblOffset + (partFirstTriIndices[partIdx] * sizeof(Triangle)), std::ios_base::beg);
        Utils::SafeRead(ifstream, carParts[partIdx].normals.begin(), carParts[partIdx].normals.end());
    }
}