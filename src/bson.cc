#include "bson.hh"

#include <cassert>
#include <cstdint>
#include <ios>
#include <memory>

bson_document::bson_document(std::istream& s)
{
    // Add exception on EOF to prevent parsing errors
    s.exceptions(std::ios_base::failbit | std::ios_base::eofbit);

    std::uint32_t size;

    s.read(reinterpret_cast<char*>(&size), sizeof(size));

    while (true)
    {
        char c = s.get();

        if (!c)
        {
            // Allow EOF here (in case of 0)
            s.exceptions(std::ios_base::failbit);

            return;
        }
    }
}

void bson_document::dump(void) const
{
    std::assert(!"Not implemented");
}

bson::bson(std::istream& s)
{
    while (s.peek() != std::istream::traits_type::eof())
        docs_.emplace_back(std::make_shared<bson_document>(s));
}

void bson::dump(void) const
{
    for (auto doc: docs_)
        doc->dump();
}
