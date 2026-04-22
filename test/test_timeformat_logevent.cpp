#include <AsynLogger/TimeFormatter.hpp>
#include <iostream>

void TestTimeFormat()
{
    LogEvent event(LogLevel::INFO, "test", 20, "test", "FormatTimeTest");
    DefaultFromatter dformat;
    std::string timeStr = dformat.format(event, SecondPrecision::MilliSeconds);
    std::cout << "当前格式化时间：" << timeStr << std::endl;
}