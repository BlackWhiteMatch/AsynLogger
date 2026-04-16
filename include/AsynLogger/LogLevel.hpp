#pragma once
#include <string>
#include <cstdint>
// 日志等级
enum class LogLevel : uint8_t{
    TRACE = 0,
    DEBUG = 1,
    INFO = 2,
    WARNING = 3,
    ERROR = 4,
    FATAL = 5
};

std::string LogLevelToString(LogLevel level);