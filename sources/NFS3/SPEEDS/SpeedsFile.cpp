#include <limits>

#include "NFS3/SPEEDS/SpeedsFile.h"

using namespace LibOpenNFS::NFS3;

void SpeedsFile::SerializeIn(std::istream& ifstream)
{
    // Tactical grab of the file size
    ifstream.ignore(std::numeric_limits<std::streamsize>::max());
    m_uFileSize = ifstream.gcount();
    ifstream.clear();
    ifstream.seekg(0, std::ios_base::beg);

    speeds.resize(m_uFileSize);
    Utils::SafeRead(ifstream, speeds.begin(), speeds.end());
}

void SpeedsFile::SerializeOut(std::ostream& ofstream) const
{
    ofstream.write((char*) speeds.data(), m_uFileSize);
}