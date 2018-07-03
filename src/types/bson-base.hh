#pragma once

#include <cstdint>
#include <ostream>

class bson;

/* abstract */ class bson_element
{
public:
    virtual void dump(std::ostream& s) const = 0;
    virtual std::uint32_t size(void) const = 0;

protected:
    bson_element() = default;

    bson_element(const bson_element&) = delete;
    bson_element& operator=(const bson_element&) = delete;
};

template <char Id>
/* abstract */ class bson_element_base: public bson_element
{
    friend bson;

    static constexpr char id(void)
    {
        return Id;
    }
};
