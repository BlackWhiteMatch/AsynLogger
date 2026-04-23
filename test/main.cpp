#include <iostream>
#include <string>

// 声明创建测试目录函数
void CreateTestLogDir();
void CreateStrategyTestDir();

// 基础功能测试
void TestConsole();
void TestSingleFile();
void TestRollingFile();

// 版本全量测试
void TestCxx11_All();
#if __cplusplus >= 201703L
void TestCxx17_All();
#endif
#if __cplusplus >= 202002L
void TestCxx20_All();
#endif

// WriteStrategy 测试
void TestSyncWriteStrategy();
void TestAsyncWriteStrategy();
void TestAsyncHighConcurrency();
void TestAsyncWithRollingFile();

int main() {
    // 第一步：创建测试日志文件夹
    CreateTestLogDir();
    CreateStrategyTestDir();

    std::cout << "=== 开始C++版本兼容性测试 ===" << std::endl;
    std::cout << "当前C++标准版本: " << __cplusplus << std::endl;
    std::cout << std::endl;

    // 执行测试
    TestCxx11_All();

#if __cplusplus >= 201703L
    TestCxx17_All();
#endif

#if __cplusplus >= 202002L
    TestCxx20_All();
#endif

    std::cout << std::endl;
    std::cout << "=== 开始 WriteStrategy 测试 ===" << std::endl;
    std::cout << std::endl;

    // WriteStrategy 测试
    TestSyncWriteStrategy();
    TestAsyncWriteStrategy();
    TestAsyncHighConcurrency();
    TestAsyncWithRollingFile();

    std::cout << "=== 所有测试执行完毕 ===" << std::endl;
    return 0;
}