#include "NFS5/CRP/CRPFile.h"
#include "NFS5/CRP/Detail.hpp"

#include <iterator>
#include <vector>
#include <sstream>
#include <cstring>

using namespace LibOpenNFS;
using namespace LibOpenNFS::NFS5;
using namespace LibOpenNFS::NFS5::CRP;
using namespace LibOpenNFS::NFS5::CRP::Detail;

void CRPFile::SerializeIn(std::istream &is)
{
    auto data = Decompress(is);
    auto fin = std::stringstream{};

    fin.write(data.data(), data.size());

    data.clear();

    fin.seekg(0);

    Utils::SafeRead(fin, header);

    // Articles
    articles.reserve(header.headerInfo.partsNumber);

    for (int i = 0; i < header.headerInfo.partsNumber; ++i)
    {
        fin.seekg(header.articlesOffset * 16 + i * sizeof(ArticleRead));

        auto &article = articles.emplace_back();

        ArticleRead articleRead;
        Utils::SafeRead(fin, articleRead);

        if (std::strncmp(articleRead.identifier, "itrA", sizeof(articleRead.identifier)) != 0)
            throw std::runtime_error{"Invalid article identifier"};

        article.headerInfo = articleRead.headerInfo;

        for (int i = 0; i < articleRead.partTableLength * 16; ++i)
        {
            fin.seekg(articleRead.offset * 16 + i * sizeof(PartWithoutInfo));

            PartWithoutInfo part;
            Utils::SafeRead(fin, part);

            if (std::strncmp(part.identifier, "esaB", sizeof(part.identifier)) == 0)
            {
                auto &base     = article.bases.emplace_back();
                auto &basePart = *reinterpret_cast<BasePart *>(&part);
                BaseRead baseRead;
                Utils::SafeRead(fin, baseRead);

                auto masksLength = basePart.length - sizeof(baseRead);
                if (masksLength > 0 && masksLength % sizeof(LevelInfo) != 0)
                    throw std::runtime_error{"Invalid length of masks"};

                base.levelMasks.reserve(masksLength / sizeof(LevelInfo));
                for (int i = 0; i < masksLength / sizeof(LevelInfo); ++i)
                {
                    auto &levelInfo = base.levelMasks.emplace_back();
                    Utils::SafeRead(fin, levelInfo);
                }
            }
            else if (std::strncmp(part.identifier, "emaN", sizeof(part.identifier)) == 0)
            {
                auto &namePart = *reinterpret_cast<NamePart *>(&part);
                auto &name     = article.names.emplace_back();
                name.resize(namePart.length);

                fin.seekg(namePart.offset - sizeof(NamePart), std::ios::cur);
                Utils::SafeRead(fin, name.begin(), name.end());
            }
            else
            {
                auto &partWithInfo = *reinterpret_cast<PartWithInfo *>(&part);
                if (std::strncmp(partWithInfo.identifier, "ns", sizeof(partWithInfo.identifier)) == 0)
                {
                    auto &cullingPart = *reinterpret_cast<CullingPart *>(&part);

                    fin.seekg(cullingPart.offset - sizeof(CullingPart), std::ios::cur);
                }
                else if (std::strncmp(partWithInfo.identifier, "rt", sizeof(partWithInfo.identifier)) == 0)
                {
                }
                else if (std::strncmp(partWithInfo.identifier, "tv", sizeof(partWithInfo.identifier)) == 0)
                {
                }
                else if (std::strncmp(partWithInfo.identifier, "mn", sizeof(partWithInfo.identifier)) == 0)
                {
                }
                else if (std::strncmp(partWithInfo.identifier, "vu", sizeof(partWithInfo.identifier)) == 0)
                {
                }
                else if (std::strncmp(partWithInfo.identifier, "rp", sizeof(partWithInfo.identifier)) == 0)
                {
                }
                else if (std::strncmp(partWithInfo.identifier, "fe", sizeof(partWithInfo.identifier)) == 0)
                {
                }
                else if (std::strncmp(partWithInfo.identifier, "fd", sizeof(partWithInfo.identifier)) == 0)
                {
                }
                else if (std::strncmp(partWithInfo.identifier, "nA", sizeof(partWithInfo.identifier)) == 0)
                {
                }
                else
                {
                    throw std::runtime_error{"Unknown part type"};
                }
            }
        }
    }

    // MiscParts
    miscParts.reserve(header.miscDataCount);
    for (int i = 0; i < header.miscDataCount; ++i)
        Utils::SafeRead(fin, miscParts.emplace_back());
}

bool CRPFile::IsCompressed(std::istream &is)
{
    // CRP compression type
    unsigned int id = 0;
    is.read((char *) &id, 4);

    return (id & 0x0000FFFF) != 0xFB10;
}

std::vector<char> CRPFile::Decompress(std::istream &is)
{
    is.seekg(0, std::ios::end);
    std::streamoff issize = is.tellg();
    if (is.tellg() <= 0x10)
        throw std::runtime_error{"CRP has invalid is size"};

    is.seekg(0);

    // Initialization
    std::vector<char> data;

    // CRP compression type
    unsigned int id = 0;
    is.read((char *) &id, 4);

    // Uncompressed CRP
    if ((id & 0x0000FFFF) != 0xFB10)
    {
        data = std::vector<char>{std::istreambuf_iterator<char>{is}, std::istreambuf_iterator<char>{}};
    }
    // Compressed CRP
    else
    {
        // Create uncompressed data array
        is.seekg(2);

        unsigned int elhi, elmd, ello;
        elhi = elmd = ello = 0;
        is.read((char *) &elhi, 1);
        is.read((char *) &elmd, 1);
        is.read((char *) &ello, 1);

        data.resize((((elhi * 256) + elmd) * 256) + ello);
        // if(length <= 0)
        //     throw std::runtime_error{"Unable to allocate buffer for decompressed CRP data! Invalid length read"};

        // Initialization
        is.seekg(5);
        unsigned int datapos = 0, len = 0, offset = 0, inbyte = 0, tmp1 = 0, tmp2 = 0, tmp3 = 0;
        char *srcpos, *dstpos;

        is.read(reinterpret_cast<char *>(&inbyte), 1);

        // Decompress
        while ((!is.eof()) && (inbyte < 0xFC))
        {
            if (!(inbyte & 0x80))
            {
                is.read(reinterpret_cast<char *>(&tmp1), 1);
                len = inbyte & 0x03;
                if (len != 0)
                {
                    is.read(&data[datapos], len);
                    datapos += len;
                }
                len = ((inbyte & 0x1C) >> 2) + 3;
                if (len != 0)
                {
                    offset = ((inbyte >> 5) << 8) + tmp1 + 1;
                    dstpos = &data[datapos];
                    srcpos = &data[datapos - offset];
                    datapos += len;
                    while (len--)
                        *dstpos++ = *srcpos++;
                }
            }
            else if (!(inbyte & 0x40))
            {
                is.read((char *) &tmp1, 1);
                is.read((char *) &tmp2, 1);
                len = (tmp1 >> 6) & 0x03;
                if (len != 0)
                {
                    is.read(&data[datapos], len);
                    datapos += len;
                }
                len = (inbyte & 0x3F) + 4;
                if (len != 0)
                {
                    offset = ((tmp1 & 0x3F) * 256) + tmp2 + 1;
                    srcpos = &data[datapos - offset];
                    dstpos = &data[datapos];
                    datapos += len;
                    while (len--)
                        *dstpos++ = *srcpos++;
                }
            }
            else if (!(inbyte & 0x20))
            {
                is.read(reinterpret_cast<char *>(&tmp1), 1);
                is.read(reinterpret_cast<char *>(&tmp2), 1);
                is.read(reinterpret_cast<char *>(&tmp3), 1);

                len = inbyte & 0x03;
                if (len != 0)
                {
                    is.read(&data[datapos], len);
                    datapos += len;
                }

                len = (((inbyte >> 2) & 0x03) * 256) + tmp3 + 5;
                if (len != 0)
                {
                    offset = ((inbyte & 0x10) << 0x0C) + (tmp1 * 256) + tmp2 + 1;
                    srcpos = &data[datapos - offset];
                    dstpos = &data[datapos];
                    datapos += len;
                    while (len--)
                        *dstpos++ = *srcpos++;
                }
            }
            else
            {
                len = ((inbyte & 0x1F) * 4) + 4;
                if (len != 0)
                {
                    is.read(&data[datapos], len);
                    datapos += len;
                }
            }

            inbyte = tmp1 = tmp2 = tmp3 = 0;
            is.read(reinterpret_cast<char *>(&inbyte), 1);
        }

        if ((!is.eof()) && (datapos < data.size()))
        {
            len = inbyte & 0x03;
            if (len != 0)
                is.read(&data[datapos], len);
        }
    }

    return data;
}