#pragma once

#include <string>
#include <optional>

namespace scsr
{

std::optional<std::string> ReadFile(const std::string& path);

}