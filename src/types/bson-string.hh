#pragma once

#include <cstdint>
#include <istream>
#include <ostream>
#include <string>

#include "bson-base.hh"

class bson_string: public bson_element_base<0x2>
{
public:
    bson_string(std::istream& s);

    void dump(std::ostream& s) const final;

    std::uint32_t size(void) const final
    {
        return sizeof(std::uint32_t) + str_.length() + 1;
    }

private:
    std::uint8_t type_;

    std::string str_;
};
