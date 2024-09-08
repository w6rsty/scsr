#include "core/io.hpp"
#include "core/log.hpp"

#include <fstream>
#include <chrono>

namespace scsr
{

Timer::Timer(const std::string& name) :
    m_Name(name),
    m_StartTime(0.0),
    m_EndTime(0.0),
    m_CallStack(s_CallStack)
{
    if (s_CallStack == 0)
    {
        PRINT(">>> Timer stack");
    }
    s_CallStack++;
    m_StartTime = std::chrono::duration<f64>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}
Timer::~Timer()
{
    m_EndTime = std::chrono::duration<f64>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    s_CallStack--;

    f64 duration = (m_EndTime - m_StartTime) * 1000.0;
    std::string indent = "";
    for (usize i = 0; i < m_CallStack; i++)
    {
        indent += "----";
    }
    PRINT("|{:>2}|{:>9.4f}ms|{}/{}", s_CallStack, duration, indent, m_Name);
}

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