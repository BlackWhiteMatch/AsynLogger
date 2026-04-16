#include "AsynLogger/LogLevel.hpp"

// 日志转字符串
std::string LogLevelToString(LogLevel level){
    switch (level)
    {
    case LogLevel::TRACE: return "TRACE";
    case LogLevel::DEBUG: return "DEBUG";
    case LogLevel::INFO: return "INFO";
    case LogLevel::WARNING: return "WARNING";
    case LogLevel::ERROR: return "ERROR";
    case LogLevel::FATAL: return "FATAL";
    default: return "UNKONWN";
    }
}