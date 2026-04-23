#pragma once
#include <iostream>
#include <thread>
#include <mutex>
#include <fstream>
#include <stdexcept>

#if __cplusplus >= 201103L
#include <sys/stat.h>
#include <cstdio>
#endif

#if __cplusplus >= 201703L
#include <string_view>
#include <filesystem>
#endif

#if __cplusplus >= 202002L
#include <format>
#endif

class LogSink{
public:
    virtual ~LogSink() = default;
    virtual void Sink(const std::string& formattedLog) = 0;
    virtual bool Flush() = 0;
    // 异常抛出函数
    void ThrowOpenError(const std::string& filePath) const;
};

// 控制台输出
class ConsoleSink : public LogSink {
public:
    void Sink(const std::string& formattedLog) override;
    bool Flush() override;
private:
    std::mutex _mutex;
};

// 单文件输出
class FileSink : public LogSink {
public:
#if __cplusplus >= 201703L
    explicit FileSink(std::string_view filePath);
#elif __cplusplus >= 201103L
    explicit FileSink(const std::string& filePath);
#endif

    void Sink(const std::string& formattedLog) override;
    bool Flush() override;
    ~FileSink() override;
private:
    std::string _filePath;
    std::ofstream _ofs;
    std::mutex _mutex;
};

// 滚动文件输出
class RollingFileSink : public LogSink{
public:
    RollingFileSink(
        const std::string& basePath,
        size_t maxFileSize,
        size_t maxFileNum,
        bool isOpenNewFile
    );
    
    void Sink(const std::string& formattedLog) override;
    bool Flush() override;
    ~RollingFileSink() override;
private:
    std::mutex _mutex;
    std::ofstream _ofs;
    std::string _basePath;
    size_t _maxFileSize;
    size_t _maxFileNum;
    size_t _currentSize = 0;

    void RollFile();
};

// Sink工厂
class SinkFactory {
public:
    static std::unique_ptr<LogSink> CreateConsoleSink(){
        return std::make_unique<ConsoleSink>();
    }
#if __cplusplus >= 201703L
    static std::unique_ptr<LogSink> CreateFileSink(std::string_view filePath){
        return std::make_unique<FileSink>(filePath);
    }
#elif __cplusplus >= 201103L
    static std::unique_ptr<LogSink> CreateFileSink(const std::string& filePath){
        return std::make_unique<FileSink>(filePath);
    }
#endif
    static std::unique_ptr<LogSink> CreateRollingFileSink(
        const std::string& basePath,
        size_t maxFileSize,
        size_t maxFileNum,
        bool isOpenNewFile
    ){
        return std::make_unique<RollingFileSink>(basePath, maxFileSize, maxFileNum, isOpenNewFile);
    }
};