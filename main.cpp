#include <iostream>
#include <fstream>

#include "NFS2/COL/ColFile.h"
#include "NFS2/PSH/PshFile.h"
#include "NFS2/TRK/TrkFile.h"

int main()
{
    LibOpenNFS::NFS2::TrkFile<LibOpenNFS::NFS2::Platform::PC> a;

    std::ifstream fin{"tr00.trk", std::ios::binary};

    fin >> a;

    std::cout << "asd";
}