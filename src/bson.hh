#pragma once

#include <istream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

/* interface */ class bson_element
{
public:
    virtual void dump(void) const = 0;

protected:
    bson_element() = default;

    bson_element(const bson_element&) = delete;
    bson_element& operator=(const bson_element&) = delete;
};

class bson_document: public bson_element
{
public:
    bson_document(std::istream& s);

    void dump(void) const final;

private:
    std::unordered_map<std::string, std::shared_ptr<bson_element>> elems_;
};

class bson
{
public:
    bson(std::istream& s);

    void dump(void) const;

    auto size(void)
    {
        return docs_.size();
    }

private:
    std::vector<std::shared_ptr<bson_document>> docs_;
};
