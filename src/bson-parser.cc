#include <cerrno>
#include <fstream>
#include <iostream>
#include <system_error>

#include "bson.hh"

static inline auto load_bson(char* file_name)
{
    std::ifstream f(file_name);

    if (!f)
        throw std::system_error(errno, std::system_category());

    // Throw an exception if IO fails
    f.exceptions(std::ifstream::failbit);

    // No use of std::vector<char> here for performance issues
    return bson(f);
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <BSON file>" << std::endl;

        return 1;
    }

#ifdef NDEBUG
    try {
#endif
        // Try parsing all BSON documents from the BSON file.
        auto bson_docs = load_bson(argv[1]);

        std::cout << "The specified BSON file contains "
            << bson_docs.size() << " contiguous BSON documents."
            << std::endl;

        // Dump all BSON documents
        bson_docs.dump(std::cout);
#ifdef NDEBUG
    }
    catch (std::runtime_error& ex)
    {
        std::cerr << "An error occured: " << ex.what() << std::endl;

        return 1;
    }
#endif

    return 0;
}
