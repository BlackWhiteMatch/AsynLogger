#pragma once
#include <iostream>
#include <chrono>
#include <string>
#include <thread>
#include "AsynLogger/LogLevel.hpp"

struct LogEvent {
public:
    LogLevel level;
    std::string fileName;
    int line;
    std::chrono::system_clock::time_point timeStamp;
    std::thread::id thread_id;
    std::string logFileName;
    std::string message;

#if __cplusplus >= 201703L
    LogEvent(
        LogLevel lv,
        std::string_view fn, int l,
        std::string_view logf,
        std::string_view msg
    ) : level(lv), fileName(fn), line(l), logFileName(logf), message(msg),
    timeStamp(std::chrono::system_clock::now()), thread_id(std::this_thread::get_id()){}
#else
    LogEvent(
        LogLevel lv,
        const std::string& fn, int l,
        const std::string& logf,
        const std::string& msg
    ) : level(lv), fileName(fn), line(l), logFileName(logf), message(msg),
    timeStamp(std::chrono::system_clock::now()), thread_id(std::this_thread::get_id()){}
#endif
};