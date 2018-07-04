#pragma once

#include <cstdint>
#include <istream>
#include <ostream>

#include "bson-base.hh"

template <const char* Name, std::uint8_t Id>
class bson_dummy: public bson_element_base<Id>
{
public:
    bson_dummy(std::istream& s)
    {
        (void)s;
    }

    void dump(std::ostream& s) const final
    {
        static constexpr std::string_view name(Name);

        s << name;
    }

    std::uint32_t size(void) const final
    {
        return 0;
    }
};
