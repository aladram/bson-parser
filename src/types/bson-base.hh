#pragma once

#include <cstdint>
#include <ostream>

/* abstract */ class bson_element
{
public:
    virtual void dump(std::ostream& s) const = 0;

    virtual std::uint32_t size(void) const = 0;
};

template <char Id>
/* abstract */ class bson_element_base: public bson_element
{
public:
    static constexpr char id(void)
    {
        return Id;
    }
};
