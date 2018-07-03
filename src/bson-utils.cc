#include "bson-utils.hh"

#include <istream>
#include <ostream>
#include <string>

#include "bson-base.hh"

std::ostream& operator<<(std::ostream& os, const bson_element& e)
{
    e.dump(os);

    return os;
}

std::ostream& operator<<(std::ostream& os, const bson_element* e)
{
    e->dump(os);

    return os;
}

std::string extract_cstring(std::istream& s)
{
    std::string str;

    while (char c = s.get())
        str.push_back(c);

    return str;
}
