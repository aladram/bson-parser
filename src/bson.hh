#pragma once

#include <functional>
#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

/* abstract */ class bson_element
{
public:
    virtual void dump(std::ostream& s) const = 0;

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

private:
    T t_;
};

const char name_double[] = "bson_double";
using bson_double = bson_generic<double, name_double, 0x1>;

class bson_document: public bson_element_base<0x3>
{
public:
    bson_document(std::istream& s);

    void dump(std::ostream& s) const final;

private:
    std::unordered_map<std::string, std::shared_ptr<bson_element>> elems_;
};

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
			BSON_ELEM(bson_document)
		};

#ifdef NDEBUG
		return map[id];
#else
		return map.at(id);
#endif
	}

private:
    std::vector<std::shared_ptr<bson_document>> docs_;
};
