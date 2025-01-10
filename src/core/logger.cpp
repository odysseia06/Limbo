#include "core/logger.h"
#include <iostream>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <thread>

namespace LimboEngine
{
    // This could be a local static pointer or static instance
    static std::mutex loggerMutex;

    Logger& Logger::Get()
    {
        static Logger instance;
        return instance;
    }

	Logger::Logger()
		: m_Running(true)
	{
		// Start the logging thread
		m_Thread = std::thread(&Logger::ThreadRun, this);
	}

	Logger::~Logger()
	{
        {
            m_Running = false;

			std::unique_lock<std::mutex> lock(m_Mutex);
			m_CondVar.notify_one();
        }

		if (m_Thread.joinable())
		{
			m_Thread.join();
		}
	}

    void Logger::Log(LogLevel level, const std::string& message, const char* file, int line)
    {
		std::ostringstream oss;

		auto now = std::chrono::system_clock::now();
		std::time_t tt = std::chrono::system_clock::to_time_t(now);
        std::tm localTm;

#if defined(_WIN32)
		localtime_s(&localTm, &tt);
#else
		localtime_r(&tt, &localTm);
#endif

		oss << "[" << std::put_time(&localTm, "%Y-%m-%d %H:%M:%S") << "] "
			<< "[" << LevelToString(level) << "] "
			<< "[TID:" << std::this_thread::get_id() << "] "
			<< "[" << file << ":" << line << "] "
			<< message;

		{
			std::unique_lock<std::mutex> lock(m_Mutex);
			m_MessageQueue.push(oss.str());
		}

		m_CondVar.notify_one();
    }

	void Logger::ThreadRun() 
	{
		while (m_Running)
		{
			std::unique_lock<std::mutex> lock(m_Mutex);

			m_CondVar.wait(lock, [this] { return !m_MessageQueue.empty() || !m_Running; });

			while (!m_MessageQueue.empty())
			{
				std::string front = std::move(m_MessageQueue.front());
				m_MessageQueue.pop();

				lock.unlock();

				std::cout << front << std::endl;

				lock.lock();
			}
		}
		// Drain any final messages
		while (!m_MessageQueue.empty())
		{
			std::string front = std::move(m_MessageQueue.front());
			m_MessageQueue.pop();
			std::cout << front << std::endl;
		}
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
