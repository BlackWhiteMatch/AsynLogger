# AsynLogger

🎯 轻量级、跨 C++ 标准的异步日志库（开发中）

## 项目简介

AsynLogger 是一个无第三方依赖、兼容 C++11/C++20 双标准的轻量级 C++ 日志库，旨在提供简单易用、工业级规范的日志功能。

## ✨ 已实现特性

- ✅ 兼容 C++11 / C++20 自动切换时间格式化方案
- ✅ 支持 秒 / 毫秒 / 微秒 / 纳秒 4 种自定义时间精度
- ✅ 完整 LogLevel 日志级别枚举（DEBUG/INFO/WARN/ERROR/FATAL）
- ✅ 日志级别转字符串功能
- ✅ 模块化单元测试
- ✅ CMake 跨平台构建支持

## 🚧 待开发

- 🚧 LogEvent 日志事件结构体
- 🚧 日志格式化输出
- 🚧 日志宏封装（LOG_INFO/LOG_ERROR）
- 🚧 控制台彩色输出
- 🚧 线程安全保证
- 🚧 文件日志输出
- 🚧 异步日志核心

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

### 时间格式化

```cpp
#include <AsynLogger/TimeFormat.hpp>
#include <iostream>

int main() {
    std::cout << "秒级时间：" << FormatTime(SecondPrecision::Seconds) << std::endl;
    std::cout << "毫秒级时间：" << FormatTime(SecondPrecision::MilliSeconds) << std::endl;
    std::cout << "微秒级时间：" << FormatTime(SecondPrecision::MicroSeconds) << std::endl;
    std::cout << "纳秒级时间：" << FormatTime(SecondPrecision::NanoSeconds) << std::endl;
    return 0;
}
```

### 日志级别转换

```cpp
#include <AsynLogger/LogLevel.hpp>
#include <iostream>

int main() {
    std::cout << "DEBUG: " << LogLevelToString(LogLevel::DEBUG) << std::endl;
    std::cout << "INFO:  " << LogLevelToString(LogLevel::INFO) << std::endl;
    std::cout << "WARN:  " << LogLevelToString(LogLevel::WARN) << std::endl;
    std::cout << "ERROR: " << LogLevelToString(LogLevel::ERROR) << std::endl;
    std::cout << "FATAL: " << LogLevelToString(LogLevel::FATAL) << std::endl;
    return 0;
}
```

------

## 🧪 运行测试

```base
cd build
./tests
```

------

## 📂 项目结构

```
AsynLogger/
├── include/AsynLogger/       # 对外暴露的核心头文件
│   ├── AsynLogger.hpp       # 总入口头文件
│   ├── LogLevel.hpp         # 日志级别枚举与转换声明
│   └── TimeFormat.hpp       # 时间格式化函数声明
├── src/                      # 核心源码实现
│   ├── LogLevel.cpp         # 日志级别转换实现
│   └── TimeFormat.cpp       # 时间格式化实现
├── example/                  # 用户示例代码
│   └── demo.cpp              # 最简单的使用示例
├── test/                     # 单元测试代码
│   ├── test_loglevel.cpp     # 日志级别转换测试
│   └── test_time.cpp         # 时间格式化测试
├── build/                    # 编译目录（不上传GitHub）
├── CMakeLists.txt            # CMake 构建配置
├── .gitignore                # Git 忽略文件
├── README.md                 # 项目说明书
└── LICENSE                   # MIT 开源协议
```

------

## 🛠️ 编译要求

- C++11 及以上
- CMake 3.14 及以上

------

## 📝 后续规划

-  实现 LogEvent 日志事件结构体
-  实现日志格式化输出
-  封装日志宏（LOG_INFO/LOG_ERROR）
-  支持控制台彩色输出
-  实现线程安全打印
-  支持文件日志输出
-  实现异步日志核心

------

## 📄 开源协议

MIT License