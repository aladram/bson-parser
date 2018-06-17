#include "bson.hh"

#include <cstdint>
#include <ios>
#include <istream>
#include <memory>
#include <ostream>
#include <string>

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

static std::string extract_cstring(std::istream& s)
{
    std::string str;

    while (char c = s.get())
        str.push_back(c);

    return str;
}

bson_document::bson_document(std::istream& s)
{
    // Add exception on EOF to prevent parsing errors
    s.exceptions(std::ios_base::failbit | std::ios_base::eofbit);

    std::uint32_t size = bson_generic<std::uint32_t>(s).value();

    //DEBUG
    //s.read(std::string(size - 5, '\0').data(), size - 5);
    //END DEBUG

    for (size -= sizeof(size); size; --size)
    {
        char c = s.get();

        if (!c)
        {
            if (size != 1)
                throw std::runtime_error("Incorrect size");

            // Allow EOF here (in case of 0)
            s.exceptions(std::ios_base::failbit);

            return;
        }

        std::string name = extract_cstring(s);

        // ...
    }

    throw std::runtime_error("Document exceeds given size");
}

void bson_document::dump(std::ostream& s) const
{
    s << "bson_document (" << elems_.size() << " fields) {" << std::endl;

    for (auto const& [key, value]: elems_)
        s << "\"" << key << "\": " << value << std::endl;

    s << "}" << std::endl;
}

bson::bson(std::istream& s)
{
    while (s.peek() != std::istream::traits_type::eof())
        docs_.emplace_back(std::make_shared<bson_document>(s));
}

void bson::dump(std::ostream& s) const
{
    for (auto doc: docs_)
        s << doc;
}
