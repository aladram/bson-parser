#include "bson.hh"

#include <cassert>
#include <vector>
#include <unordered_map>

bson_document::bson(std::istream& s)
{
    // Add exception on EOF to prevent parsing errors
    s.exceptions(std::ifstream::failbit | std::ifstream::eofbit);

    while (true)
    {
        // Allow EOF here (in case of 0)
        s.exceptions(std::ifstream::failbit);

        char c = s.peek();

        if (!c)
            return;

        // Restore normal state
        s.exceptions(std::ifstream::failbit | std::ifstream::eofbit);

        assert(c == s.get());
    }
}

void bson_document::dump(void)
{
    assert(!"Not implemented");
}

bson::bson(std::istream& s)
{
    while (!s.eof())
        docs_.emplace_back(bson_document(s));
}

void bson::dump(void)
{
    for (auto doc: docs_)
        doc.dump();
}
