#pragma once

#include "core/type.hpp"

#include <string>
#include <optional>

namespace scsr
{

static usize s_CallStack = 0;

class Timer
{
public:
    Timer(const std::string& name);
    ~Timer();
private:
    std::string m_Name;
    f64 m_StartTime;
    f64 m_EndTime;
    usize m_CallStack;
};

#ifdef SCSR_PROFILINGa
    #define PROFILE_SCOPE(name) Timer timer(name)
#else
    #define PROFILE_SCOPE(name)
#endif

std::optional<std::string> ReadFile(const std::string& path);

}