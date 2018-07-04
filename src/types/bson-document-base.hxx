#include <cstdint>
#include <ios>
#include <istream>
#include <ostream>
#include <string>

#include "bson.hh"
#include "bson-generic.hh"

template <typename T, typename U, std::uint8_t V>
bson_document_base<T, U, V>::bson_document_base(std::istream& s)
{
    // Add exception on EOF to prevent parsing errors
    s.exceptions(std::ios_base::failbit | std::ios_base::eofbit);

    size_ = bson_generic<std::uint32_t>(s).value();

    auto size = sizeof(size_);

    while (size < size_)
    {
        char c = s.get();
        
        ++size;

        if (!c)
        {
            if (size != size_)
                throw std::runtime_error("Incorrect size");

            // Allow EOF here (in case of 0)
            s.exceptions(std::ios_base::failbit);

            return;
        }

        std::string name = extract_cstring(s);

        /*
         * Compiler may remove this call if policy is no_name_policy and
         * optimizations are on (because of final keyword on validate_name)
         */
        name_policy_.validate_name(name);

        size += name.length() + 1;

        auto elem = bson_factory(static_cast<std::uint8_t>(c))(s);

        size += elem->size();

        elems_[std::move(name)] = std::move(elem);
    }

    throw std::runtime_error("Document exceeds given size");
}

template <typename T, typename U, std::uint8_t V>
void bson_document_base<T, U, V>::dump(std::ostream& s) const
{
    s << print_policy_.name << "(" << elems_.size() << " fields) "
        << print_policy_.open_bracket << std::endl;

    {
        auto it = elems_.begin();

        while (true)
        {
            print_policy_.print(s, it->first, it->second);

            if (++it == elems_.end())
            {
                s << std::endl;

                break;
            }

            s << "," << std::endl;
        }
    }

    s << print_policy_.close_bracket;
}
