#include <cstring>

#include "NFS3/FFN/FfnFile.h"

//#include "../../../Util/ImageLoader.h"

using namespace LibOpenNFS::NFS3;

static uint32_t ARGB1555toARGB8888(uint16_t c)
{
    const uint32_t a = c & 0x8000, r = c & 0x7C00, g = c & 0x03E0, b = c & 0x1F;
    const uint32_t rgb = (r << 9) | (g << 6) | (b << 3);
    return (a * 0x1FE00) | rgb | ((rgb >> 5) & 0x070707);
}

void FfnFile::SerializeIn(std::istream &ifstream)
{
    // Get filesize so can check have parsed all bytes
    Utils::SafeRead(ifstream, header);

    if (strncmp(header.fntfChk, "FNTF", sizeof(header.fntfChk)) != 0)
        throw std::runtime_error{"Invalid FFN Header"};

    characters.resize(header.numChars);
    Utils::SafeRead(ifstream, characters.begin(), characters.end());

    uint32_t predictedAFontOffset = header.fontMapOffset;

    header.numChars = 400;
    header.version  = 164;

    // streamoff readBytes = ffn.tellg();
    // ASSERT(readBytes == header->fileSize, "Missing " << header.fileSize - readBytes << " bytes from loaded FFN file: " << ffn_path);

    ifstream.seekg(header.fontMapOffset, std::ios_base::beg);
    std::vector<uint32_t> pixels(header.version * header.numChars);
    std::vector<uint16_t> paletteColours(0xFF);
    std::vector<uint8_t> indices(header.version * header.numChars); // Only used if indexed

    for (int pal_Idx = 0; pal_Idx < 255; ++pal_Idx)
        paletteColours[pal_Idx] = 65535;

    for (int y = 0; y < header.numChars; y++)
        for (int x = 0; x < header.version; x++)
            Utils::SafeRead(ifstream, indices[(x + y * header.version)]);

    // Rewrite the pixels using the palette data
    for (int y = 0; y < header.numChars; y++)
        for (int x = 0; x < header.version; x++)
        {
            uint32_t pixel                   = ARGB1555toARGB8888(paletteColours[indices[(x + y * header.version)]]);
            pixels[(x + y * header.version)] = pixel;
        }

    // ImageLoader::SaveImage("C:/Users/Amrik/Desktop/test.bmp", pixels.data(), header.version, header.numChars);

    // ASSERT(readBytes == header.fileSize, "Missing " << header.fileSize - readBytes << " bytes from loaded FFN file: " << ffn_path);
}