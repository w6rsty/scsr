#include "core/io.hpp"

#include <fstream>

namespace scsr
{

std::optional<std::string> ReadFile(const std::string& path)
{
    std::ifstream file(path.data());
    if (!file.is_open())
    {
        return std::nullopt;
    }

    std::string content;
    std::string line;
    while (std::getline(file, line))
    {
        content += line + "\n";
    }
    return std::make_optional(content);
}

}