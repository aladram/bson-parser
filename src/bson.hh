#pragma once

#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

/* interface */ class bson_element
{
public:
    virtual void dump(std::ostream& s) const = 0;

protected:
    bson_element() = default;

    bson_element(const bson_element&) = delete;
    bson_element& operator=(const bson_element&) = delete;
};

template <typename T>
class bson_generic: public bson_element
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
        s << t_;
    }

private:
    T t_;
};

class bson_document: public bson_element
{
public:
    bson_document(std::istream& s);

    void dump(std::ostream& s) const final;

private:
    std::unordered_map<std::string, std::shared_ptr<bson_element>> elems_;
};

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
    std::vector<std::shared_ptr<bson_document>> docs_;
};
