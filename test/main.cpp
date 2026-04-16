#include "AsynLogger/AsynLogger.hpp"
#include <iostream>

// 声明测试函数
void TestTimeFormat();
void TestLogLevel();

int main() {
    std::cout << "=== 开始运行所有单元测试 ===" << std::endl;
    
    TestTimeFormat();
    std::cout << "✅ 时间格式化测试通过" << std::endl << std::endl;
    
    TestLogLevel();
    std::cout << "✅ 日志级别测试通过" << std::endl;
    
    std::cout << "=== 所有测试全部通过！===" << std::endl;
    return 0;
}