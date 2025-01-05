#include "core/logger.h"
#include <iostream>
#include <mutex>

namespace LimboEngine
{
    // This could be a local static pointer or static instance
    static std::mutex loggerMutex;

    Logger& Logger::Get()
    {
        static Logger instance;
        return instance;
    }

    void Logger::Log(LogLevel level, const std::string& message, const char* file, int line)
    {
        std::lock_guard<std::mutex> lock(loggerMutex);

        // Convert the log level to a string
        std::string levelStr = LevelToString(level);

        // For now, just print to standard output
        std::cout << "[" << levelStr << "] "
            << "[" << file << ":" << line << "] "
            << message << std::endl;
    }

    std::string Logger::LevelToString(LogLevel level)
    {
        switch (level)
        {
        case LogLevel::Trace:    return "TRACE";
        case LogLevel::Debug:    return "DEBUG";
        case LogLevel::Info:     return "INFO";
        case LogLevel::Warn:     return "WARN";
        case LogLevel::Error:    return "ERROR";
        case LogLevel::Critical: return "CRITICAL";
        }
        return "UNKNOWN";
    }
}
