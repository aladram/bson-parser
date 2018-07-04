#include "bson-string.hh"

#include <cctype>
#include <cstdint>
#include <iomanip>
#include <istream>
#include <ostream>
#include <stdexcept>

#include "bson-generic.hh"

bson_string::bson_string(std::istream& s)
{
    std::uint32_t size = bson_generic<std::uint32_t>(s).value();

    if (!size)
        throw std::runtime_error("bson_string with null size (\\0 required)");

    --size;

    str_.resize(size);

    s.read(str_.data(), size);

    if (s.get())
        throw std::runtime_error("bson_string not null-terminated");
}

void bson_string::dump(std::ostream& s) const
{
    s << "bson_string(\"";

    for (char c: str_)
    {
        if (c == '"')
            s << "\\\"";

        else if (c == '\\')
            s << "\\\\";

        else if (std::isprint(c))
            s << c;

        else
        {
            auto old_flags = s.flags();

            s << "\\x"
                << std::hex << std::setfill('0') << std::setw(2)
                << static_cast<int>(c);

            s.flags(old_flags);
        }
    }

    s << "\")";
}
