#include "NFS5/VIV/VIVFile.h"

#include <filesystem>
#include <cstring>
#include <fstream>

using namespace LibOpenNFS::NFS5;

static uint32_t SwapEndian(uint32_t x)
{
    int swapped;

    swapped = (x >> 24) | ((x << 8) & 0x00FF0000) | ((x >> 8) & 0x0000FF00) | (x << 24);

    return static_cast<uint32_t>(swapped);
}

void VIVFile::SerializeIn(std::istream &is)
{
    char isHeader[4];
    is.read((char *) isHeader, sizeof(isHeader));
    if (strncmp(isHeader, "BIGF", sizeof(isHeader)))
        throw std::runtime_error{"Invalid file (BIGF header missing)"};

    uint32_t isSize;
    Utils::SafeRead(is, isSize);
    isSize = SwapEndian(isSize);

    uint32_t nFiles;
    Utils::SafeRead(is, nFiles);
    nFiles = SwapEndian(nFiles);

    uint32_t startPos;
    Utils::SafeRead(is, startPos);
    startPos = SwapEndian(startPos);

    std::streampos currentPos = is.tellg();

    for (unsigned fileIdx = 0; fileIdx < nFiles; ++fileIdx)
    {
        is.seekg(currentPos, std::ios_base::beg);

        uint32_t filePos;
        Utils::SafeRead(is, filePos);
        filePos = SwapEndian(filePos);

        uint32_t fileSize;
        Utils::SafeRead(is, fileSize);
        fileSize = SwapEndian(fileSize);

        auto &bnk = files.emplace_back();
        std::getline(is, bnk.name, '\0');
        bnk.data.resize(fileSize);

        currentPos = is.tellg();
        is.seekg(filePos, std::ios_base::beg);
        is.read(bnk.data.data(), bnk.data.size());
    }
}