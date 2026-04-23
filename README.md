# AsynLogger

🎯 轻量级、跨 C++ 标准的异步日志库

## 项目简介

AsynLogger 是一个无第三方依赖、兼容 C++11/C++17/C++20 的轻量级 C++ 日志库，旨在提供简单易用、工业级规范的日志功能。

## ✨ 已实现特性

- ✅ 兼容 C++11 / C++17 / C++20 自动切换时间格式化方案
- ✅ 支持 秒 / 毫秒 / 微秒 / 纳秒 4 种自定义时间精度
- ✅ 完整 LogLevel 日志级别枚举（TRACE/DEBUG/INFO/WARNING/ERROR/FATAL）
- ✅ 日志级别转字符串功能
- ✅ LogEvent 日志事件结构体（包含时间戳、线程ID、文件名、行号等）
- ✅ TimeFormatter 时间格式化器（支持多种精度）
- ✅ LogSink 日志输出系统
  - ConsoleSink 控制台输出
  - FileSink 单文件输出
  - RollingFileSink 滚动文件输出（按大小自动归档）
- ✅ WriteStrategy 写入策略系统
  - SyncWriteStrategy 同步写入策略
  - AsyncWriteStrategy 异步写入策略（生产者-消费者模型）
- ✅ 线程安全保证（所有 Sink 均使用 mutex 保护）
- ✅ 异步日志队列核心（支持批量写入、优雅退出）
- ✅ 模块化单元测试
- ✅ CMake 跨平台构建支持

## 🚧 待开发

- 🚧 Logger 核心日志类实现
- 🚧 日志宏封装（LOG_INFO/LOG_ERROR/LOG_WARN 等）
- 🚧 控制台彩色输出
- 🚧 日志格式化模板系统

------

## 🚀 快速开始

### 1. 引入头文件

```cpp
#include <AsynLogger/AsynLogger.hpp>
```

### 2. 编译

```bash
# 创建构建目录
mkdir build && cd build

# CMake 配置
cmake ..

# 编译
cmake --build .
```

------

## 📌 示例代码

### 控制台日志输出

```cpp
#include <AsynLogger/LogSink.hpp>
#include <iostream>

int main() {
    ConsoleSink console;
    console.Sink("[INFO] 程序正常运行");
    console.Sink("[WARN] 控制台日志输出正常");
    console.Flush();
    return 0;
}
```

### 单文件日志输出

```cpp
#include <AsynLogger/LogSink.hpp>
#include <iostream>

int main() {
    try {
        FileSink fileSink("app.log");
        fileSink.Sink("[INFO] 普通文件日志内容1");
        fileSink.Sink("[ERROR] 普通文件错误日志内容2");
        fileSink.Flush();
    } catch (const std::exception& e) {
        std::cout << "文件异常: " << e.what() << std::endl;
    }
    return 0;
}
```

### 滚动文件日志（自动归档）

```cpp
#include <AsynLogger/LogSink.hpp>
#include <iostream>

int main() {
    try {
        // 参数：基础路径 | 单文件最大字节 | 最大保留历史文件 | 启动新建文件
        RollingFileSink rollSink("roll_log.log", 1024 * 1024, 5, true);
        
        for (int i = 1; i <= 100; ++i) {
            std::string log = "[INFO] 滚动日志测试序号：" + std::to_string(i);
            rollSink.Sink(log);
        }
        
        rollSink.Flush();
        std::cout << "日志已满自动滚动备份完成" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "滚动日志异常: " << e.what() << std::endl;
    }
    return 0;
}
```

### 同步写入策略

```cpp
#include <AsynLogger/WriteStrategy.hpp>
#include <AsynLogger/LogSink.hpp>
#include <iostream>

int main() {
    // 创建 Sinks
    std::vector<std::unique_ptr<LogSink>> sinks;
    sinks.push_back(std::make_unique<ConsoleSink>());
    sinks.push_back(std::make_unique<FileSink>("app.log"));

    // 创建同步策略
    SyncWriteStrategy strategy(sinks);

    // 写入日志（主线程直接写入）
    strategy.Write("[INFO] 同步日志消息1");
    strategy.Write("[INFO] 同步日志消息2");
    strategy.Flush();

    return 0;
}
```

### 异步写入策略

```cpp
#include <AsynLogger/WriteStrategy.hpp>
#include <AsynLogger/LogSink.hpp>
#include <iostream>

int main() {
    // 创建 Sinks
    std::vector<std::unique_ptr<LogSink>> sinks;
    sinks.push_back(std::make_unique<ConsoleSink>());
    sinks.push_back(std::make_unique<FileSink>("async.log"));

    // 创建异步策略（队列大小 1000）
    AsyncWriteStrategy strategy(sinks, 1000);

    // 启动后台线程
    strategy.Start();

    // 写入日志（非阻塞，入队）
    for (int i = 0; i < 100; ++i) {
        strategy.Write("[INFO] 异步日志消息 #" + std::to_string(i));
    }

    // 等待所有日志写完
    strategy.Flush();

    // 停止后台线程
    strategy.Stop();

    return 0;
}
```

### 日志级别转换

```cpp
#include <AsynLogger/LogLevel.hpp>
#include <iostream>

int main() {
    std::cout << "TRACE:   " << LogLevelToString(LogLevel::TRACE) << std::endl;
    std::cout << "DEBUG:   " << LogLevelToString(LogLevel::DEBUG) << std::endl;
    std::cout << "INFO:    " << LogLevelToString(LogLevel::INFO) << std::endl;
    std::cout << "WARNING: " << LogLevelToString(LogLevel::WARNING) << std::endl;
    std::cout << "ERROR:   " << LogLevelToString(LogLevel::ERROR) << std::endl;
    std::cout << "FATAL:   " << LogLevelToString(LogLevel::FATAL) << std::endl;
    return 0;
}
```

### LogEvent 日志事件

```cpp
#include <AsynLogger/LogEvent.hpp>
#include <AsynLogger/TimeFormatter.hpp>
#include <iostream>

int main() {
    LogEvent event(
        LogLevel::INFO,
        "main.cpp",
        42,
        "app.log",
        "这是一条测试日志"
    );
    
    DefaultFromatter formatter;
    std::string timeStr = formatter.format(event, SecondPrecision::MilliSeconds);
    std::cout << timeStr << std::endl;
    
    return 0;
}
```

------

## 🧪 运行测试

```bash
cd build
./tests
```

测试包含：
- 日志级别转换测试
- 时间格式化测试
- Sink 兼容性测试（控制台/文件/滚动文件）
- 同步写入策略测试
- 异步写入策略测试
- 高并发写入测试

------

## 📂 项目结构

```
AsynLogger/
├── include/AsynLogger/          # 对外暴露的核心头文件
│   ├── AsynLogger.hpp          # 总入口头文件
│   ├── LogLevel.hpp            # 日志级别枚举与转换声明
│   ├── LogEvent.hpp            # 日志事件结构体
│   ├── LogSink.hpp             # 日志输出 Sink（控制台/文件/滚动文件）
│   ├── WriteStrategy.hpp       # 写入策略（同步/异步）
│   └── TimeFormatter.hpp       # 时间格式化器声明
├── src/                         # 核心源码实现
│   ├── LogLevel.cpp            # 日志级别转换实现
│   ├── LogSink.cpp             # 日志 Sink 实现
│   ├── WriteStrategy.cpp       # 写入策略实现
│   └── TimeFormatter.cpp       # 时间格式化实现
├── example/                     # 用户示例代码
│   └── demo.cpp                 # 完整使用示例
├── test/                        # 单元测试代码
│   ├── main.cpp                 # 测试主入口
│   ├── test_loglevel.cpp        # 日志级别转换测试
│   ├── test_sink_compat.cpp     # 日志 Sink 兼容性测试
│   ├── test_timeformat_logevent.cpp  # 时间格式化与日志事件测试
│   └── test_write_strategy.cpp  # 写入策略测试
├── build/                       # 编译目录（不上传GitHub）
├── CMakeLists.txt               # CMake 构建配置
├── .gitignore                   # Git 忽略文件
├── README.md                    # 项目说明书
└── LICENSE                      # MIT 开源协议
```

------

## 🏗️ 核心架构

### 1. LogSink（日志输出器）
负责将格式化后的日志写入不同目标：
- `ConsoleSink`: 控制台输出
- `FileSink`: 单文件追加输出
- `RollingFileSink`: 按大小自动滚动归档

所有 Sink 都是线程安全的（使用 mutex 保护）。

### 2. WriteStrategy（写入策略）
控制日志的写入方式：
- `SyncWriteStrategy`: 同步写入，调用线程直接写入所有 Sink
- `AsyncWriteStrategy`: 异步写入，生产者-消费者模型，后台线程批量写入

**异步策略特性**：
- 非阻塞入队（队列满时等待）
- 批量写入（减少锁竞争）
- 优雅退出（Stop 时处理剩余日志）
- 支持 Flush（等待队列清空）

### 3. 设计模式
- **策略模式**: 同步/异步策略可切换
- **工厂模式**: SinkFactory 创建不同类型的 Sink
- **RAII**: 资源自动管理，析构时自动清理

------

## 🛠️ 编译要求

- C++11 及以上（推荐 C++17 或 C++20）
- CMake 3.14 及以上
- 支持平台：Windows / Linux / macOS

------

## 📝 后续规划

- 实现 Logger 核心日志类
- 封装日志宏（LOG_INFO/LOG_ERROR/LOG_WARN 等）
- 支持控制台彩色输出（ANSI 转义码）
- 支持自定义日志格式化模板
- 性能优化与压力测试
- 支持日志过滤器

------

## 📄 开源协议

MIT License
