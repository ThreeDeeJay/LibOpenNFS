#include "NFS3/FCE/FceFile.h"

using namespace LibOpenNFS::NFS3;

void FceFile::SerializeIn(std::istream &is)
{
    Utils::SafeRead(is, unknown);
    Utils::SafeRead(is, nTriangles);
    Utils::SafeRead(is, nVertices);
    Utils::SafeRead(is, nArts);
    Utils::SafeRead(is, vertTblOffset);
    Utils::SafeRead(is, normTblOffset);
    Utils::SafeRead(is, triTblOffset);
    Utils::SafeRead(is, reserve1Offset);
    Utils::SafeRead(is, reserve2Offset);
    Utils::SafeRead(is, reserve3Offset);
    Utils::SafeRead(is, modelHalfSize);
    Utils::SafeRead(is, nDummies);
    Utils::SafeRead(is, dummyCoords);
    Utils::SafeRead(is, nParts);
    Utils::SafeRead(is, partCoords);
    Utils::SafeRead(is, partFirstVertIndices);
    Utils::SafeRead(is, partNumVertices);
    Utils::SafeRead(is, partFirstTriIndices);
    Utils::SafeRead(is, partNumTriangles);
    Utils::SafeRead(is, nPriColours);
    Utils::SafeRead(is, primaryColours);
    Utils::SafeRead(is, nSecColours);
    Utils::SafeRead(is, secondaryColours);
    Utils::SafeRead(is, dummyNames);
    Utils::SafeRead(is, partNames);
    Utils::SafeRead(is, unknownTable);

    carParts.resize(nParts);

    for (uint32_t partIdx = 0; partIdx < nParts; ++partIdx)
    {
        carParts[partIdx].vertices.resize(partNumVertices[partIdx]);
        is.seekg(0x1F04 + vertTblOffset + (partFirstVertIndices[partIdx] * sizeof(glm::vec3)), std::ios_base::beg);
        Utils::SafeRead(is, carParts[partIdx].vertices.begin(), carParts[partIdx].vertices.end());

        carParts[partIdx].normals.resize(partNumVertices[partIdx]);
        is.seekg(0x1F04 + normTblOffset + (partFirstVertIndices[partIdx] * sizeof(glm::vec3)), std::ios_base::beg);
        Utils::SafeRead(is, carParts[partIdx].normals.begin(), carParts[partIdx].normals.end());

        carParts[partIdx].triangles.resize(partNumTriangles[partIdx]);
        is.seekg(0x1F04 + triTblOffset + (partFirstTriIndices[partIdx] * sizeof(Triangle)), std::ios_base::beg);
        Utils::SafeRead(is, carParts[partIdx].normals.begin(), carParts[partIdx].normals.end());
    }
}