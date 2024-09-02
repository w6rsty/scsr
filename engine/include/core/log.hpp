#pragma once

#include "core/type.hpp"
#ifdef SCSR_LOGGING

#include <map>
#include <mutex>
#include <chrono>
#include <string>
#include <string_view>

#include "fmt/core.h"

namespace scsr
{

enum class Level
{
    None = 0,
    Debug,
    Info,
    Warn,
    Error,
};

static const std::string LevelToString(Level level)
{
    switch (level)
    {
        case Level::Debug:  return "Debug";
        case Level::Info:   return "Info";
        case Level::Warn:   return "Warn";
        case Level::Error:  return "Error";
        default:            return "None";
    }
}

static constexpr std::string_view s_TimeFormat = "{:%FT%T}";

class Logger
{
public:
    Logger(const std::string& name = "Default", Level filter = Level::None) :
        m_FilterLevel(filter)
    {
        auto now = std::chrono::system_clock::now();
        auto tz = std::chrono::current_zone();
        std::chrono::zoned_time local_time{tz, now};

        fmt::println("{} [{}] Logger [{}] created",
            std::format(s_TimeFormat, local_time), LevelToString(Level::Info), name);
    }

    void SetFilterLevel(Level level) { m_FilterLevel = level; }
    Level GetFilterLevel() const { return m_FilterLevel; }

    template <typename... Args>
    void Log(Level level, std::string_view file, int line,  fmt::format_string<Args...> format, Args&&... args) {
        if (level < m_FilterLevel) {
            return;
        }
        
        std::lock_guard<std::mutex> lock(m_Mutex);

        auto now = std::chrono::system_clock::now();
        auto tz = std::chrono::current_zone();
        std::chrono::zoned_time local_time{tz, now};

        fmt::print("{} [{}] {}:{}: ",
            std::format(s_TimeFormat, local_time), LevelToString(level), file, line);
        fmt::println(format, std::forward<Args>(args)...);
    }

private:
    std::mutex m_Mutex;
    Level m_FilterLevel;
};

class LoggerManager
{
public:
    static LoggerManager& Instance()
    {
        static Scp<LoggerManager> instance;
        if (!instance)
        {
            instance.reset(new LoggerManager());
        }
        return *instance;
    }

    Logger& Get(const std::string& name)
    {
        StringHash hash = StrToHash(name);
        auto it = m_Loggers.find(hash);
        if (it == m_Loggers.end())
        {
            it = m_Loggers.emplace(hash, MakeScp<Logger>(name)).first;
        }

        return *(it->second);
    }
private:
    std::map<StringHash, Scp<Logger>> m_Loggers;
};

#ifdef SCSR_LOGGING
    #define LOGGER ::scsr::LoggerManager::Instance().Get("engine")

    #define LOG_DEBUG(...) LOGGER.Log(::scsr::Level::Debug, __FILE__, __LINE__, __VA_ARGS__)
    #define LOG_INFO( ...) LOGGER.Log(::scsr::Level::Info,  __FILE__, __LINE__, __VA_ARGS__)
    #define LOG_WARN( ...) LOGGER.Log(::scsr::Level::Warn,  __FILE__, __LINE__, __VA_ARGS__)
    #define LOG_ERROR(...) LOGGER.Log(::scsr::Level::Error, __FILE__, __LINE__, __VA_ARGS__)
#else
    #define LOGGER

    #define LOG_DEBUG(...)
    #define LOG_INFO( ...)
    #define LOG_WARN( ...)
    #define LOG_ERROR(...)
#endif

}

#ifdef SCSR_LOGGING
    #define LOGGER_RT ::scsr::LoggerManager::Instance().Get("runtime")

    #define LOG_RT_DEBUG(...) LOGGER_RT.Log(::scsr::Level::Debug, __FILE__, __LINE__, __VA_ARGS__)
    #define LOG_RT_INFO( ...) LOGGER_RT.Log(::scsr::Level::Info,  __FILE__, __LINE__, __VA_ARGS__)
    #define LOG_RT_WARN( ...) LOGGER_RT.Log(::scsr::Level::Warn,  __FILE__, __LINE__, __VA_ARGS__)
    #define LOG_RT_ERROR(...) LOGGER_RT.Log(::scsr::Level::Error, __FILE__, __LINE__, __VA_ARGS__)
#else
    #define LOGGER_RT

    #define LOG_RT_DEBUG(...)
    #define LOG_RT_INFO( ...)
    #define LOG_RT_WARN( ...)
    #define LOG_RT_ERROR(...)
#endif

#endif
