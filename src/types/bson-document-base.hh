#pragma once

#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>

#include "bson-base.hh"
#include "bson-utils.hh"

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

#include "bson-document-base.hxx"
