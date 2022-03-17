#include "NFS3/FRD/PolyBlock.h"

using namespace LibOpenNFS::NFS3;

void PolyBlock::SerializeIn(std::istream &ifstream)
{
    for (uint32_t polyBlockIdx = 0; polyBlockIdx < cNumPolygonBlocks; polyBlockIdx++)
    {
        Utils::SafeRead(ifstream, sz[polyBlockIdx]);
        if (sz[polyBlockIdx] != 0)
        {
            Utils::SafeRead(ifstream, szdup[polyBlockIdx]);
            if (szdup[polyBlockIdx] != sz[polyBlockIdx])
                throw;

            poly[polyBlockIdx] = std::vector<PolygonData>(sz[polyBlockIdx]);
            Utils::SafeRead(ifstream, poly[polyBlockIdx].begin(), poly[polyBlockIdx].end());
        }
    }

    // Sanity check
    if (sz[4] != m_nTrackBlockPolys)
        throw;

    for (auto &o : obj)
    {
        Utils::SafeRead(ifstream, o.n1);
        if (o.n1 > 0)
        {
            Utils::SafeRead(ifstream, o.n2);

            o.types.resize(o.n2);
            o.numpoly.resize(o.n2);
            o.poly.resize(o.n2);

            uint32_t polygonCount = 0;
            o.nobj                = 0;

            for (uint32_t k = 0; k < o.n2; ++k)
            {
                Utils::SafeRead(ifstream, o.types[k]);

                if (o.types[k] == 1)
                {
                    Utils::SafeRead(ifstream, o.numpoly[o.nobj]);

                    o.poly[o.nobj] = std::vector<PolygonData>(o.numpoly[o.nobj]);
                    Utils::SafeRead(ifstream, o.poly[o.nobj].begin(), o.poly[o.nobj].end());

                    polygonCount += o.numpoly[o.nobj];
                    ++o.nobj;
                }
            }
            // n1 == total nb polygons
            if (polygonCount != o.n1)
                throw;
        }
    }
}

void PolyBlock::SerializeOut(std::ostream &ofstream) const
{
    for (uint32_t polyBlockIdx = 0; polyBlockIdx < cNumPolygonBlocks; polyBlockIdx++)
    {
        ofstream.write((char *) &sz[polyBlockIdx], sizeof(uint32_t));
        if (sz[polyBlockIdx] != 0)
        {
            ofstream.write((char *) &szdup[polyBlockIdx], sizeof(uint32_t));
            ofstream.write((char *) poly[polyBlockIdx].data(), sizeof(PolygonData) * sz[polyBlockIdx]);
        }
    }

    for (auto &o : obj)
    {
        ofstream.write((char *) &o.n1, sizeof(uint32_t));
        if (o.n1 > 0)
        {
            ofstream.write((char *) &o.n2, sizeof(uint32_t));
            uint32_t nobj = 0;
            for (uint32_t k = 0; k < o.n2; ++k)
            {
                ofstream.write((char *) &o.types[k], sizeof(uint32_t));
                if (o.types[k] == 1)
                {
                    ofstream.write((char *) &o.numpoly[o.nobj], sizeof(uint32_t));
                    ofstream.write((char *) o.poly[o.nobj].data(), sizeof(PolygonData) * o.numpoly[o.nobj]);
                    ++nobj;
                }
            }
        }
    }
}
