#pragma once

#include <cstdint>
#include <istream>
#include <ostream>
#include <string_view>

template <typename T, const char* Name = nullptr, char Id = 0>
class bson_generic: public bson_element_base<Id>
{
public:
    bson_generic(std::istream& s)
    {
        s.read(reinterpret_cast<char*>(&t_), sizeof(T));
    }

    T value() const
    {
        return t_;
    }

    void dump(std::ostream& s) const final
    {
        static constexpr std::string_view name(Name);

        s << name << "(" << t_ << ")";
    }

    std::uint32_t size(void) const final
    {
        return sizeof(T);
    }

private:
    T t_;
};
