#pragma once

#include <cstdint>
#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "bson-base.hh"
#include "bson-dummy.hh"
#include "bson-generic.hh"
#include "bson-utils.hh"

#define NEW_BSON_DUMMY(Id, Name) \
static const char name_ ## Name[] = #Name; \
using Name = bson_dummy<name_ ## Name, Id>

NEW_BSON_DUMMY(0x6, bson_undefined);
NEW_BSON_DUMMY(0xA, bson_null);
NEW_BSON_DUMMY(0xFF, bson_min_key);
NEW_BSON_DUMMY(0x7F, bson_max_key);

#define NEW_BSON_TYPE(Id, Name, Type) \
static const char name_ ## Name[] = "bson_"#Name; \
using bson_ ## Name = bson_generic<Type, name_ ## Name, Id>

static_assert(sizeof(double) == 8, "double size required to be 8 bytes");

NEW_BSON_TYPE(0x1, double, double);
NEW_BSON_TYPE(0x8, boolean, std::uint8_t);
NEW_BSON_TYPE(0x9, datetime, std::int64_t);
NEW_BSON_TYPE(0x10, int32, std::int32_t);
NEW_BSON_TYPE(0x11, timestamp, std::uint64_t);
NEW_BSON_TYPE(0x12, int64, std::int64_t);

auto bson_factory(std::uint8_t id);

#include "bson-document-base.hh"

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

private:
    std::vector<bson_document> docs_;
};
