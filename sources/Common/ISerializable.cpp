#include "Common/ISerializable.h"

std::ostream& operator<<(std::ostream& os, const ISerializable& ref)
{
    ref.SerializeOut(os);
    return os;
}

std::istream& operator>>(std::istream& is, IDeserializable& ref)
{
    ref.SerializeIn(is);
    return is;
}