#pragma once
#include <ctime>
#include <chrono>
#include <string>
#include "AsynLogger/LogEvent.hpp"

#if __cplusplus >= 202002L
#include <format>
#endif

enum class SecondPrecision{
    Seconds,
    MilliSeconds,
    MicroSeconds,
    NanoSeconds
};

class TimeFormatter {
public:
    virtual ~TimeFormatter() = default;
    virtual std::string format(const LogEvent& event, SecondPrecision Spre) const = 0;
};

class DefaultTimeFormatter : public TimeFormatter{
public:
    std::string format(const LogEvent& event, SecondPrecision Spre) const override;
};