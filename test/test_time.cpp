#include "AsynLogger/TimeFormat.hpp"
#include <iostream>

// 测试主函数
void TestTimeFormat() {
    // 测试四种时间精度
    std::cout << "秒   :" << FormatNowTime(SecondPrecision::Seconds) << std::endl;
    std::cout << "毫秒 :" << FormatNowTime(SecondPrecision::MilliSeconds) << std::endl;
    std::cout << "微秒 :" << FormatNowTime(SecondPrecision::MicroSeconds) << std::endl;
    std::cout << "纳秒 :" << FormatNowTime(SecondPrecision::NanoSeconds) << std::endl;
}