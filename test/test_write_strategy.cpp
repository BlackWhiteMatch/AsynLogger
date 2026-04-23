#include <AsynLogger/WriteStrategy.hpp>
#include <AsynLogger/LogSink.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <sys/stat.h>

// 测试日志目录
const std::string STRATEGY_TEST_DIR = "test_logs";

// 创建测试目录
void CreateStrategyTestDir() {
#ifdef _WIN32
    _mkdir(STRATEGY_TEST_DIR.c_str());
#else
    mkdir(STRATEGY_TEST_DIR.c_str(), 0755);
#endif
}

// 测试同步写入策略
void TestSyncWriteStrategy() {
    std::cout << "=====================================" << std::endl;
    std::cout << "测试同步写入策略 (SyncWriteStrategy)" << std::endl;
    std::cout << "=====================================" << std::endl;

    try {
        // 创建 Sinks
        std::vector<std::unique_ptr<LogSink>> sinks;
        sinks.push_back(std::make_unique<ConsoleSink>());
        sinks.push_back(std::make_unique<FileSink>(STRATEGY_TEST_DIR + "/sync_test.log"));

        // 创建同步策略
        SyncWriteStrategy strategy(sinks);

        // 写入日志
        std::cout << "开始同步写入测试..." << std::endl;
        for (int i = 1; i <= 5; ++i) {
            std::string log = "[SYNC] 同步日志消息 #" + std::to_string(i);
            strategy.Write(log);
        }

        // 刷新
        strategy.Flush();
        std::cout << "同步写入测试完成" << std::endl;

    } catch (const std::exception& e) {
        std::cout << "同步策略测试异常: " << e.what() << std::endl;
    }
    std::cout << std::endl;
}

// 测试异步写入策略
void TestAsyncWriteStrategy() {
    std::cout << "=====================================" << std::endl;
    std::cout << "测试异步写入策略 (AsyncWriteStrategy)" << std::endl;
    std::cout << "=====================================" << std::endl;

    try {
        // 创建 Sinks
        std::vector<std::unique_ptr<LogSink>> sinks;
        sinks.push_back(std::make_unique<ConsoleSink>());
        sinks.push_back(std::make_unique<FileSink>(STRATEGY_TEST_DIR + "/async_test.log"));

        // 创建异步策略
        AsyncWriteStrategy strategy(sinks, 1000);

        // 启动后台线程
        std::cout << "启动异步写入线程..." << std::endl;
        strategy.Start();

        // 写入日志
        std::cout << "开始异步写入测试..." << std::endl;
        for (int i = 1; i <= 10; ++i) {
            std::string log = "[ASYNC] 异步日志消息 #" + std::to_string(i);
            strategy.Write(log);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        // 等待所有日志写完
        std::cout << "等待日志队列清空..." << std::endl;
        strategy.Flush();

        // 停止后台线程
        std::cout << "停止异步写入线程..." << std::endl;
        strategy.Stop();

        std::cout << "异步写入测试完成" << std::endl;

    } catch (const std::exception& e) {
        std::cout << "异步策略测试异常: " << e.what() << std::endl;
    }
    std::cout << std::endl;
}

// 测试异步策略高并发写入
void TestAsyncHighConcurrency() {
    std::cout << "=====================================" << std::endl;
    std::cout << "测试异步策略高并发写入" << std::endl;
    std::cout << "=====================================" << std::endl;

    try {
        // 创建 Sinks
        std::vector<std::unique_ptr<LogSink>> sinks;
        sinks.push_back(std::make_unique<FileSink>(STRATEGY_TEST_DIR + "/async_concurrent.log"));

        // 创建异步策略
        AsyncWriteStrategy strategy(sinks, 5000);
        strategy.Start();

        // 多线程并发写入
        std::cout << "启动 5 个线程并发写入..." << std::endl;
        std::vector<std::thread> threads;
        for (int t = 0; t < 5; ++t) {
            threads.emplace_back([&strategy, t]() {
                for (int i = 0; i < 20; ++i) {
                    std::string log = "[THREAD-" + std::to_string(t) + "] 消息 #" + std::to_string(i);
                    strategy.Write(log);
                }
            });
        }

        // 等待所有线程完成
        for (auto& th : threads) {
            th.join();
        }

        std::cout << "所有线程写入完成，等待队列清空..." << std::endl;
        strategy.Flush();
        strategy.Stop();

        std::cout << "高并发测试完成 (共写入 100 条日志)" << std::endl;

    } catch (const std::exception& e) {
        std::cout << "高并发测试异常: " << e.what() << std::endl;
    }
    std::cout << std::endl;
}

// 测试异步策略与滚动文件
void TestAsyncWithRollingFile() {
    std::cout << "=====================================" << std::endl;
    std::cout << "测试异步策略 + 滚动文件" << std::endl;
    std::cout << "=====================================" << std::endl;

    try {
        // 创建 Sinks
        std::vector<std::unique_ptr<LogSink>> sinks;
        sinks.push_back(std::make_unique<RollingFileSink>(
            STRATEGY_TEST_DIR + "/async_rolling.log",
            200,  // 200 字节触发滚动
            3,    // 保留 3 个历史文件
            true
        ));

        // 创建异步策略
        AsyncWriteStrategy strategy(sinks);
        strategy.Start();

        // 写入大量日志触发滚动
        std::cout << "写入日志触发文件滚动..." << std::endl;
        for (int i = 1; i <= 20; ++i) {
            std::string log = "[ROLLING] 异步滚动日志测试消息 #" + std::to_string(i);
            strategy.Write(log);
        }

        strategy.Flush();
        strategy.Stop();

        std::cout << "异步滚动文件测试完成" << std::endl;

    } catch (const std::exception& e) {
        std::cout << "异步滚动文件测试异常: " << e.what() << std::endl;
    }
    std::cout << std::endl;
}
