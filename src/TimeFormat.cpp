#include "AsynLogger/TimeFormat.hpp"

std::string FormatNowTime(SecondPrecision Spre){
    auto now = std::chrono::system_clock::now();

#if __cplusplus >= 202002L
    switch (Spre)
    {
    case SecondPrecision::Seconds:
        return std::format(
            "{:%Y-%m-%d %H:%M:%S}",
            std::chrono::floor<std::chrono::seconds>(now)
        );
    case SecondPrecision::MilliSeconds:
        return std::format(
            "{:%Y-%m-%d %H:%M:%S}",
            std::chrono::floor<std::chrono::milliseconds>(now)
        );
    case SecondPrecision::MicroSeconds:
        return std::format(
            "{:%Y-%m-%d %H:%M:%S}",
            std::chrono::floor<std::chrono::microseconds>(now)
        );
    case SecondPrecision::NanoSeconds:
        return std::format(
            "{:%Y-%m-%d %H:%M:%S}",
            std::chrono::floor<std::chrono::nanoseconds>(now)
        );
    default:
        return std::format(
            "{:%Y-%m-%d %H:%M:%S}",
            std::chrono::floor<std::chrono::seconds>(now)
        );
    }
#endif
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    auto local_time = *std::localtime(&now_time_t);
    auto sub_sec = now - std::chrono::system_clock::from_time_t(now_time_t);
    long long tail  = 0;
    bool need_tail = true;

    switch (Spre)
    {
    case SecondPrecision::Seconds:
        need_tail = false;
        tail = 0;
        break;
    case SecondPrecision::MilliSeconds:
        tail = std::chrono::duration_cast<std::chrono::milliseconds>(sub_sec).count();
        break;
    case SecondPrecision::MicroSeconds:
        tail = std::chrono::duration_cast<std::chrono::microseconds>(sub_sec).count();
        break;
    case SecondPrecision::NanoSeconds:
        tail = std::chrono::duration_cast<std::chrono::nanoseconds>(sub_sec).count();
        break;
    default:
        need_tail = false;
        tail = 0;
        break;
    }

    char time_str[64];
    std::strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", &local_time);
    if(!need_tail) return std::string(time_str);
    return std::string(time_str) + "." + std::to_string(tail);
}