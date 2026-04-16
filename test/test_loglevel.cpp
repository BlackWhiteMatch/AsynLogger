#include "AsynLogger/LogLevel.hpp"
#include <iostream>

void TestLogLevel(){
    std::cout << LogLevelToString(LogLevel::TRACE) << std::endl;
    std::cout << LogLevelToString(LogLevel::DEBUG) << std::endl;
    std::cout << LogLevelToString(LogLevel::INFO) << std::endl;
    std::cout << LogLevelToString(LogLevel::WARNING) << std::endl;
    std::cout << LogLevelToString(LogLevel::ERROR) << std::endl;
    std::cout << LogLevelToString(LogLevel::FATAL) << std::endl;
}