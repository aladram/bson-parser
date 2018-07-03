#pragma once

#include <functional>
#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "bson-base.hh"

std::string extract_cstring(std::istream& s);

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

#define NEW_BSON_TYPE(Id, Name, Type) \
static const char name_ ## Name[] = "bson_"#Name; \
using bson_ ## Name = bson_generic<Type, name_ ## Name, Id>

static_assert(sizeof(double) == 8, "double size required to be 8 bytes");

NEW_BSON_TYPE(0x1, double, double);
NEW_BSON_TYPE(0x8, boolean, std::uint8_t);
NEW_BSON_TYPE(0x10, int32, std::int32_t);
NEW_BSON_TYPE(0x11, timestamp, std::uint64_t);
NEW_BSON_TYPE(0x12, int64, std::int64_t);

class bson_regex: bson_element_base<0xB>
{
public:
    bson_regex(std::istream& s)
        : pattern_(extract_cstring(s))
        , options_(extract_cstring(s))
    {}

    void dump(std::ostream& s) const final
    {
        s << "bson_regex(pattern: " << pattern_
            << ", options: " << options_ << ")";
    }

    std::uint32_t size(void) const final
    {
        return pattern_.size() + 1 + options_.size() + 1;
    }

private:
    std::string pattern_;
    std::string options_;
};

#define BSON_ELEM(Type) \
{ Type::id(), [] (std::istream& s) { return std::make_shared<Type>(s); } }

template <typename T, typename U, char V>
class bson_document_base;

class no_name_policy;
class numeric_name_policy;

class document_print_policy;
class array_print_policy;

using bson_document =
    bson_document_base<no_name_policy, document_print_policy, 0x3>;
using bson_array =
    bson_document_base<numeric_name_policy, array_print_policy, 0x4>;

class bson
{
public:
    bson(std::istream& s);

    void dump(std::ostream& s) const;

    auto size(void)
    {
        return docs_.size();
    }

    static auto factory(char id);

private:
    std::vector<std::shared_ptr<bson_document>> docs_;
};
