#pragma once

#include <string>
#include <memory>
#include <format>
#include <atomic>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace LimboEngine
{
    enum class LogLevel
    {
        Trace,
        Debug,
        Info,
        Warn,
        Error,
        Critical
    };

    class Logger
    {
    public:
        // Get a global logger instance (simple singleton pattern)
        static Logger& Get();

        // Log a message at a given level
        void Log(LogLevel level, const std::string& message, const char* file, int line);

        ~Logger();

		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;
    private:
        // Private constructor for singleton
        Logger();

        // Utility method to convert LogLevel -> string
        std::string LevelToString(LogLevel level);

        // Dedicated function run by the logging thread
        void ThreadRun();

        std::atomic<bool> m_Running;
		std::thread m_Thread;
		std::queue<std::string> m_MessageQueue;
		std::mutex m_Mutex;
		std::condition_variable m_CondVar;
    };

    //
    // Variadic macros that accept a format string plus additional args.
    // They automatically call Logger::Log(...) with file and line info.
    //
#define LIMBO_TRACE(fmtStr, ...) \
    ::LimboEngine::Logger::Get().Log( \
        ::LimboEngine::LogLevel::Trace, \
        std::format(fmtStr, __VA_ARGS__), \
        __FILE__, \
        __LINE__ \
    )

#define LIMBO_DEBUG(fmtStr, ...) \
    ::LimboEngine::Logger::Get().Log( \
        ::LimboEngine::LogLevel::Debug, \
        std::format(fmtStr, __VA_ARGS__), \
        __FILE__, \
        __LINE__ \
    )

#define LIMBO_INFO(fmtStr, ...) \
    ::LimboEngine::Logger::Get().Log( \
        ::LimboEngine::LogLevel::Info, \
        std::format(fmtStr, __VA_ARGS__), \
        __FILE__, \
        __LINE__ \
    )

#define LIMBO_WARN(fmtStr, ...) \
    ::LimboEngine::Logger::Get().Log( \
        ::LimboEngine::LogLevel::Warn, \
        std::format(fmtStr, __VA_ARGS__), \
        __FILE__, \
        __LINE__ \
    )

#define LIMBO_ERROR(fmtStr, ...) \
    ::LimboEngine::Logger::Get().Log( \
        ::LimboEngine::LogLevel::Error, \
        std::format(fmtStr, __VA_ARGS__), \
        __FILE__, \
        __LINE__ \
    )

#define LIMBO_CRITICAL(fmtStr, ...) \
    ::LimboEngine::Logger::Get().Log( \
        ::LimboEngine::LogLevel::Critical, \
        std::format(fmtStr, __VA_ARGS__), \
        __FILE__, \
        __LINE__ \
    )
} // namespace LimboEngine
