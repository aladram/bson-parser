#pragma once

#include <istream>
#include <ostream>
#include <string>

#include "bson-base.hh"

std::ostream& operator<<(std::ostream& os, const bson_element& e);

std::ostream& operator<<(std::ostream& os, const bson_element* e);

std::string extract_cstring(std::istream& s);
