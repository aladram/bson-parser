#pragma once

#include <functional>
#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

std::string extract_cstring(std::istream& s);

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

class bson;

template <char Id>
/* abstract */ class bson_element_base: public bson_element
{
    friend bson;

    static constexpr char id(void)
    {
        return Id;
    }
};

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

/* interface */ class name_policy
{
    virtual void validate_name(const std::string& name) = 0;
};

class no_name_policy: public name_policy
{
public:
    void validate_name(const std::string& name) final
    {
        (void)name;
    }
};

class numeric_name_policy: public name_policy
{
public:
    void validate_name(const std::string& name) final
    {
        if (name != std::to_string(i_))
            throw std::runtime_error("");

        ++i_;
    }

private:
    std::uint32_t i_ = 0;
};

/* interface */ class print_policy
{
    virtual void print(
        std::ostream& s,
        const std::string& key,
        const std::shared_ptr<bson_element>& value
    ) const = 0;
};

class document_print_policy: public print_policy
{
public:
   void print(
        std::ostream& s,
        const std::string& key,
        const std::shared_ptr<bson_element>& value
    ) const final
   {
       s << "\"" << key << "\": " << value;
   }

   static constexpr std::string_view name = "bson_document";
   static constexpr std::string_view open_bracket = "{";
   static constexpr std::string_view close_bracket = "}";
};

class array_print_policy: public print_policy
{
public:
   void print(
        std::ostream& s,
        const std::string& key,
        const std::shared_ptr<bson_element>& value
    ) const final
   {
       (void)key;

       s << value;
   }

   static constexpr std::string_view name = "bson_array";
   static constexpr std::string_view open_bracket = "[";
   static constexpr std::string_view close_bracket = "]";
};

template <typename NamePolicy, typename PrintPolicy, char Id>
class bson_document_base: public bson_element_base<Id>
{
    static_assert(
        std::is_base_of<name_policy, NamePolicy>::value,
        "NamePolicy is not derived from name_policy class"
    );

    static_assert(
        std::is_base_of<print_policy, PrintPolicy>::value,
        "PrintPolicy is not derived from print_policy class"
    );

public:
    bson_document_base(std::istream& s);

    void dump(std::ostream& s) const final;

    std::uint32_t size(void) const final
    {
        return size_;
    }

private:
    std::unordered_map<std::string, std::shared_ptr<bson_element>> elems_;

    std::uint32_t size_;

    NamePolicy name_policy_;

    PrintPolicy print_policy_;
};

using bson_document =
    bson_document_base<no_name_policy, document_print_policy, 0x3>;
using bson_array =
    bson_document_base<numeric_name_policy, array_print_policy, 0x4>;

#define BSON_ELEM(Type) \
{ Type::id(), [] (std::istream& s) { return std::make_shared<Type>(s); } }

class bson
{
public:
    bson(std::istream& s);

    void dump(std::ostream& s) const;

    auto size(void)
    {
        return docs_.size();
    }

    static auto factory(char id)
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

private:
    std::vector<std::shared_ptr<bson_document>> docs_;
};
