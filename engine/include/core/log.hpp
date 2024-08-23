#pragma once

#ifdef SCSR_LOGGING

#include <ctime>
#include <chrono>
#include <string>
#include <string_view>
#include <mutex>

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
        default:   return "None";
    }
}

class Logger
{
public:
    Logger(const std::string& name = "Default", Level level = Level::None)
        : m_Level(level)
    {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::tm tm;
        localtime_s(&tm, &time);

        fmt::println(
            "{:02}:{:02}:{:02} [{}] Logger [{}] created",
            tm.tm_hour, tm.tm_min, tm.tm_sec, LevelToString(level), 
            name
        );
    }

    void SetLevel(Level level) { m_Level = level; }
    Level GetLevel() const { return m_Level; }

    template <typename... Args>
    void Log(Level level, std::string_view file, int line,  fmt::format_string<Args...> format, Args&&... args) {
        if (level < m_Level) {
            return;
        }
        
        std::lock_guard<std::mutex> lock(m_Mutex);

        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::tm tm;
        localtime_s(&tm, &time);

        fmt::print(
            "{:02}:{:02}:{:02} [{}] {}:{}: ",
            tm.tm_hour, tm.tm_min, tm.tm_sec, LevelToString(level), file, line
        );
        fmt::println(format, std::forward<Args>(args)...);
    }

private:
    Level m_Level;
    std::mutex m_Mutex;
};

static Logger s_Logger;

}

#endif

#ifdef SCSR_LOGGING
    #define DEFAULT_LOGGER ::scsr::Logger s_Logger

    #define LOG_DEBUG(...) ::scsr::s_Logger.Log(::scsr::Level::Debug, __FILE__, __LINE__, __VA_ARGS__)
    #define LOG_INFO( ...) ::scsr::s_Logger.Log(::scsr::Level::Info,  __FILE__, __LINE__, __VA_ARGS__)
    #define LOG_WARN( ...) ::scsr::s_Logger.Log(::scsr::Level::Warn,  __FILE__, __LINE__, __VA_ARGS__)
    #define LOG_ERROR(...) ::scsr::s_Logger.Log(::scsr::Level::Error, __FILE__, __LINE__, __VA_ARGS__)
#else
    #define DEFAULT_LOGGER

    #define LOG_DEBUG(...)
    #define LOG_INFO( ...)
    #define LOG_WARN( ...)
    #define LOG_ERROR(...)
#endif