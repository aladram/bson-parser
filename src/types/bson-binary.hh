#pragma once

#include <cstdint>
#include <istream>
#include <ostream>
#include <string>

#include "bson-base.hh"

class bson_binary: public bson_element_base<0x5>
{
public:
    bson_binary(std::istream& s);

    void dump(std::ostream& s) const final;

    std::uint32_t size(void) const final
    {
        return sizeof(std::uint32_t) + 1 + str_.length();
    }

private:
    std::uint8_t type_;

    std::string str_;
};
