#pragma once

#include "logger.h"
#include <format>  // If you want C++20 formatting

namespace Limbo 
{
    #define LOG_TRACE(msg)   Logger::getInstance().Log(LogLevel::Trace, msg, __FILE__, __LINE__)
    #define LOG_DEBUG(msg)   Logger::getInstance().Log(LogLevel::Debug, msg, __FILE__, __LINE__)
    #define LOG_INFO(msg)    Logger::getInstance().Log(LogLevel::Info, msg, __FILE__, __LINE__)
    #define LOG_WARNING(msg) Logger::getInstance().Log(LogLevel::Warning, msg, __FILE__, __LINE__)
    #define LOG_ERROR(msg)   Logger::getInstance().Log(LogLevel::Error, msg, __FILE__, __LINE__)
    #define LOG_FATAL(msg)   Logger::getInstance().Log(LogLevel::Fatal, msg, __FILE__, __LINE__)
}