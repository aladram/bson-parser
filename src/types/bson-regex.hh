#pragma once

#include <cstdint>
#include <istream>
#include <string>

#include "bson-base.hh"
#include "bson-utils.hh"

class bson_regex: public bson_element_base<0xB>
{
public:
    bson_regex(std::istream& s)
        : pattern_(extract_cstring(s))
        , options_(extract_cstring(s))
    {}

    void dump(std::ostream& s) const final
    {
        s << "bson_regex(pattern: " << pattern_
            << ", options: " << options_ << ")";
    }

    std::uint32_t size(void) const final
    {
        return pattern_.size() + 1 + options_.size() + 1;
    }

private:
    std::string pattern_;
    std::string options_;
};
