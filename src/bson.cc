#include "bson.hh"

#include <cstdint>
#include <ios>
#include <istream>
#include <memory>
#include <ostream>
#include <string>

#include "bson-base.hh"
#include "bson-document-base.hh"

std::ostream& operator<<(std::ostream& os, bson_element& e)
{
    e.dump(os);

    return os;
}

std::ostream& operator<<(std::ostream& os, bson_element* e)
{
    e->dump(os);

    return os;
}

std::string extract_cstring(std::istream& s)
{
    std::string str;

    while (char c = s.get())
        str.push_back(c);

    return str;
}

bson::bson(std::istream& s)
{
    while (s.peek() != std::istream::traits_type::eof())
        docs_.emplace_back(std::make_shared<bson_document>(s));
}

auto bson::factory(char id)
{
    static const std::unordered_map<
        char,
        std::function<std::shared_ptr<bson_element>(std::istream&)>
    > map {
        BSON_ELEM(bson_double),
        BSON_ELEM(bson_document),
        BSON_ELEM(bson_boolean),
        BSON_ELEM(bson_int32),
        BSON_ELEM(bson_timestamp),
        BSON_ELEM(bson_int64)
    };

    return map.at(id);
}

void bson::dump(std::ostream& s) const
{
    for (auto doc: docs_)
        s << doc << std::endl;
}
