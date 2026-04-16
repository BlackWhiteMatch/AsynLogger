#include <AsynLogger/AsynLogger.hpp>

int main() {
    // 测试时间格式化
    std::cout << "秒级时间：" << FormatNowTime(SecondPrecision::Seconds) << std::endl;
    std::cout << "毫秒级时间：" << FormatNowTime(SecondPrecision::MilliSeconds) << std::endl;
    
    // 测试日志级别转换
    std::cout << "INFO级别：" << LogLevelToString(LogLevel::INFO) << std::endl;
    
    return 0;
}