#pragma once

#include "../Common/ISerializable.h"

class HrzFile : ISerializable
{
public:
    HrzFile() = default;
    static bool Load(const std::string &hrzPath, HrzFile &hrzFile);
    static void Save(const std::string &hrzPath, HrzFile &hrzFile);

    glm::vec3 skyTopColour;
    glm::vec3 skyBottomColour;

private:
    bool SerializeIn(std::istream &ifstream) override;
    void SerializeOut(std::ostream &ofstream) override;
};
