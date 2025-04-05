#pragma once
#include <string> // TODO: Make your own string class
#include <sstream>
#include <mutex>  // For std::mutex
#include <atomic>
#include <condition_variable>
#include <thread>
#include <queue>
#include <chrono>         // (optional, if we add timestamps)
#include <iomanip>        // (optional, for time formatting)
#include <iostream>

namespace Limbo 
{

    enum class LogLevel {
        Trace,
        Debug,
        Info,
        Warning,
        Error,
        Fatal
    };
    
    class Logger {
    public:
        static Logger& getInstance() {
            static Logger instance;
            return instance;
        }
    
        std::string LevelToString(LogLevel level)
        {
            switch (level)
            {
                case LogLevel::Trace:    return "TRACE";
                case LogLevel::Debug:    return "DEBUG";
                case LogLevel::Info:     return "INFO";
                case LogLevel::Warning:  return "WARNING";
                case LogLevel::Error:    return "ERROR";
                case LogLevel::Fatal:    return "FATAL";
            }
            return "UNKNOWN";
        }

        void Log(LogLevel level, const std::string& message, const char* file, int line) {
            std::lock_guard<std::mutex> lock(m_mutex);
            std::string levelStr;
            std::string colorCode;

            switch (level) {
                case LogLevel::Trace:   levelStr = "TRACE";   colorCode = "\033[0;37m"; break; // White
                case LogLevel::Debug:   levelStr = "DEBUG";   colorCode = "\033[0;34m"; break; // Blue
                case LogLevel::Info:    levelStr = "INFO";    colorCode = "\033[0;32m"; break; // Green
                case LogLevel::Warning: levelStr = "WARNING"; colorCode = "\033[0;33m"; break; // Yellow
                case LogLevel::Error:   levelStr = "ERROR";   colorCode = "\033[0;31m"; break; // Red
                case LogLevel::Fatal:   levelStr = "FATAL";   colorCode = "\033[1;31m"; break; // Bold Red
            }

            std::ostringstream oss;

            // If you want timestamps, gather them here:
            auto now       = std::chrono::system_clock::now();
            std::time_t tt = std::chrono::system_clock::to_time_t(now);
            std::tm localTm;

        #if defined(_WIN32)
            localtime_s(&localTm, &tt);
        #else
            localtime_r(&tt, &localTm);
        #endif

            oss << "[" << std::put_time(&localTm, "%Y-%m-%d %H:%M:%S") << "] "
                << "[" << LevelToString(level) << "] "
                << "[" << file << ":" << line << "] "
                << message;

            std::cout << colorCode << oss.str() << "\033[0m" << std::endl;
        }
            

    private:
        Logger() {}
        std::mutex m_mutex;
    };

}