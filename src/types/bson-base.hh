#pragma once

#include <cstdint>
#include <ostream>

/* abstract */ class bson_element
{
public:
    virtual void dump(std::ostream& s) const = 0;

    virtual std::uint32_t size(void) const = 0;
};

template <std::uint8_t Id>
/* abstract */ class bson_element_base: public bson_element
{
public:
    static constexpr std::uint8_t id(void)
    {
        return Id;
    }
};
