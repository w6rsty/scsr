#pragma once

#include "core/type.hpp"
#ifdef SCSR_LOGGING

#include <ctime>
#include <chrono>
#include <string>
#include <string_view>
#include <unordered_map>
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
    Logger(const std::string& name = "Default", Level filter = Level::None)
        : m_FilterLevel(filter)
    {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::tm tm;
        localtime_s(&tm, &time);

        fmt::println(
            "{:02}:{:02}:{:02} [{}] Logger [{}] created",
            tm.tm_hour, tm.tm_min, tm.tm_sec, LevelToString(Level::Info), 
            name
        );
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
    Level m_FilterLevel;
    std::mutex m_Mutex;
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
        auto it = m_Loggers.find(name);
        if (it == m_Loggers.end())
        {
            m_Loggers[name].reset(new Logger(name));
            it = m_Loggers.find(name);
        }

        return *(it->second);
    }
private:
    std::unordered_map<std::string, Scp<Logger>> m_Loggers;
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
