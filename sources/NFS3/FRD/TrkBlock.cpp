#include "NFS3/FRD/TrkBlock.h"

using namespace LibOpenNFS::NFS3;

void TrkBlock::SerializeIn(std::istream &frd)
{
    Utils::SafeRead(frd, ptCentre);
    Utils::SafeRead(frd, ptBounding);
    Utils::SafeRead(frd, nVertices);
    Utils::SafeRead(frd, nHiResVert);
    Utils::SafeRead(frd, nLoResVert);
    Utils::SafeRead(frd, nMedResVert);
    Utils::SafeRead(frd, nVerticesDup);
    Utils::SafeRead(frd, nObjectVert);

    if (nVertices == 0)
        throw;

    // Read Vertices
    vert.resize(nVertices);
    Utils::SafeRead(frd, vert.begin(), vert.end());

    // Read Vertices
    vertShading.resize(nVertices);
    Utils::SafeRead(frd, vertShading.begin(), vertShading.end());

    // Read neighbouring block data
    Utils::SafeRead(frd, nbdData);

    // Read trackblock metadata
    Utils::SafeRead(frd, nStartPos);
    Utils::SafeRead(frd, nPositions);
    Utils::SafeRead(frd, nPolygons);
    Utils::SafeRead(frd, nVRoad);
    Utils::SafeRead(frd, nXobj);
    Utils::SafeRead(frd, nPolyobj);
    Utils::SafeRead(frd, nSoundsrc);
    Utils::SafeRead(frd, nLightsrc);

    // Read track position data
    posData.resize(nPositions);
    Utils::SafeRead(frd, posData.begin(), posData.end());

    // Read virtual road polygons
    polyData.resize(nPolygons);
    Utils::SafeRead(frd, polyData.begin(), polyData.end());

    // Read virtual road spline data
    vroadData.resize(nVRoad);
    Utils::SafeRead(frd, vroadData.begin(), vroadData.end());

    // Read Extra object references
    xobj.resize(nXobj);
    Utils::SafeRead(frd, xobj.begin(), xobj.end());

    // ?? Read unknown
    polyObj.resize(nPolyobj);
    Utils::SafeRead(frd, polyObj.begin(), polyObj.end());
    // nPolyobj = 0;

    // Get the sound and light sources
    soundsrc.resize(nSoundsrc);
    Utils::SafeRead(frd, soundsrc.begin(), soundsrc.end());

    lightsrc.resize(nLightsrc);
    Utils::SafeRead(frd, lightsrc.begin(), lightsrc.end());
}

void TrkBlock::SerializeOut(std::ostream &frd) const
{
    frd.write((char *) &ptCentre, sizeof(glm::vec3));
    frd.write((char *) &ptBounding, sizeof(glm::vec3) * 4);
    frd.write((char *) &nVertices, sizeof(uint32_t));
    frd.write((char *) &nHiResVert, sizeof(uint32_t));
    frd.write((char *) &nLoResVert, sizeof(uint32_t));
    frd.write((char *) &nMedResVert, sizeof(uint32_t));
    frd.write((char *) &nVerticesDup, sizeof(uint32_t));
    frd.write((char *) &nObjectVert, sizeof(uint32_t));
    frd.write((char *) vert.data(), sizeof(glm::vec3) * nVertices);
    frd.write((char *) vertShading.data(), sizeof(uint32_t) * nVertices);
    frd.write((char *) nbdData, 4 * 0x12c);
    frd.write((char *) &nStartPos, sizeof(uint32_t));
    frd.write((char *) &nPositions, sizeof(uint32_t));
    frd.write((char *) &nPolygons, sizeof(uint32_t));
    frd.write((char *) &nVRoad, sizeof(uint32_t));
    frd.write((char *) &nXobj, sizeof(uint32_t));
    frd.write((char *) &nPolyobj, sizeof(uint32_t));
    frd.write((char *) &nSoundsrc, sizeof(uint32_t));
    frd.write((char *) &nLightsrc, sizeof(uint32_t));
    frd.write((char *) posData.data(), sizeof(PositionData) * nPositions);
    frd.write((char *) polyData.data(), sizeof(PolyVRoadData) * nPolygons);
    frd.write((char *) vroadData.data(), sizeof(VRoadData) * nVRoad);
    frd.write((char *) xobj.data(), sizeof(RefExtraObject) * nXobj);
    frd.write((char *) polyObj.data(), sizeof(PolyObject) * nPolyobj);
    frd.write((char *) soundsrc.data(), sizeof(SoundSource) * nSoundsrc);
    frd.write((char *) lightsrc.data(), sizeof(LightSource) * nLightsrc);
}