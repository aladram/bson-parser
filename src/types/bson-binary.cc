#include "bson-binary.hh"

#include <cstdint>
#include <iomanip>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <unordered_map>

#include "bson-generic.hh"

bson_binary::bson_binary(std::istream& s)
{
    std::uint32_t size = bson_generic<std::uint32_t>(s).value();

    type_ = bson_generic<std::uint8_t>(s).value();

    str_.resize(size);

    s.read(str_.data(), size);
}

static const std::string_view& subtype_name(std::uint8_t type)
{
    static const std::unordered_map<
        std::uint8_t,
        std::string_view
    > subtype_names{
        { 0x0, "Generic binary subtype" },
        { 0x1, "Function" },
        { 0x2, "Binary (Old)" },
        { 0x3, "UUID (Old)" },
        { 0x4, "UUID" },
        { 0x5, "MD5" },
        { 0x80, "User defined" }
    };

    return subtype_names.at(type);
}

void bson_binary::dump(std::ostream& s) const
{
    s << "bson_binary(subtype: " << subtype_name(type_) << ", data: ";

    auto old_flags = s.flags();

    s << std::hex << std::setfill('0') << std::setw(2);

    for (char c: str_)
        s << static_cast<int>(c);

    s.flags(old_flags);

    s << ")";
}
