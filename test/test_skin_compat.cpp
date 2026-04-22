#include <AsynLogger/LogSkin.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <sys/stat.h>
#include <cstdlib>

// 测试日志统一存放的文件夹名称
const std::string TEST_LOG_DIR = "test_logs";

// 跨平台创建测试日志目录
void CreateTestLogDir() {
#ifdef _WIN32
    _mkdir(TEST_LOG_DIR.c_str());
#else
    mkdir(TEST_LOG_DIR.c_str(), 0755);
#endif
    std::cout << "测试日志目录已创建: " << TEST_LOG_DIR << std::endl;
}

// 控制台日志测试
void TestConsole() {
    std::cout << "开始控制台日志测试" << std::endl;
    ConsoleSkin console;
    console.Skin("控制台日志测试信息");
    console.Skin("控制台警告测试信息");
    console.Flush();
    std::cout << "控制台日志测试完成" << std::endl;
}

// 单文件日志测试（文件存入 test_logs 目录）
void TestSingleFile() {
    std::cout << "开始单文件日志测试" << std::endl;
    std::string filePath = TEST_LOG_DIR + "/single_file_test.log";
    try {
        FileSkin file(filePath);
        file.Skin("单文件日志测试信息1");
        file.Skin("单文件日志测试信息2");
        file.Flush();
        std::cout << "单文件日志测试完成" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "单文件日志测试异常: " << e.what() << std::endl;
    }
}

// 滚动文件日志测试（文件存入 test_logs 目录）
void TestRollingFile() {
    std::cout << "开始滚动文件日志测试" << std::endl;
    std::string basePath = TEST_LOG_DIR + "/rolling_file_test.log";
    try {
        RollingFileSkin roll(basePath, 100, 3, true);
        for (int i = 0; i < 10; ++i) {
            roll.Skin("滚动日志测试内容");
        }
        roll.Flush();
        std::cout << "滚动文件日志测试完成" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "滚动文件日志测试异常: " << e.what() << std::endl;
    }
}

// C++11 全功能测试
void TestCxx11_All() {
    std::cout << "=====================================" << std::endl;
    std::cout << "执行 C++11 全功能测试" << std::endl;
    std::cout << "=====================================" << std::endl;

    TestConsole();
    std::cout << std::endl;
    TestSingleFile();
    std::cout << std::endl;
    TestRollingFile();
    std::cout << std::endl;
}

// C++17 全功能测试
#if __cplusplus >= 201703L
void TestCxx17_All() {
    std::cout << "=====================================" << std::endl;
    std::cout << "执行 C++17 全功能测试" << std::endl;
    std::cout << "=====================================" << std::endl;

    TestConsole();
    std::cout << std::endl;
    TestSingleFile();
    std::cout << std::endl;
    TestRollingFile();
    std::cout << std::endl;
}
#endif

// C++20 全功能测试
#if __cplusplus >= 202002L
void TestCxx20_All() {
    std::cout << "=====================================" << std::endl;
    std::cout << "执行 C++20 全功能测试" << std::endl;
    std::cout << "=====================================" << std::endl;

    TestConsole();
    std::cout << std::endl;
    TestSingleFile();
    std::cout << std::endl;
    TestRollingFile();
    std::cout << std::endl;
}
#endif