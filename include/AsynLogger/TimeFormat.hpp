#pragma once
#include <ctime>
#include <chrono>
#include <string>

#if __cplusplus >= 202002L
#include <format>
#endif

enum class SecondPrecision{
    Seconds,
    MilliSeconds,
    MicroSeconds,
    NanoSeconds
};

std::string FormatNowTime(SecondPrecision Spre);