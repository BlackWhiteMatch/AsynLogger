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

class LogSkin{
public:
    virtual ~LogSkin() = default;
    virtual void Skin(const std::string& formattedLog) = 0;
    virtual bool Flush() = 0;
    // 异常抛出函数
    void ThrowOpenError(const std::string& filePath) const;
};

// 控制台输出
class ConsoleSkin : public LogSkin {
public:
    void Skin(const std::string& formattedLog) override;
    bool Flush() override;
private:
    std::mutex _mutex;
};

// 单文件输出
class FileSkin : public LogSkin {
public:
#if __cplusplus >= 201703L
    explicit FileSkin(std::string_view filePath);
#elif __cplusplus >= 201103L
    explicit FileSkin(const std::string& filePath);
#endif

    void Skin(const std::string& formattedLog) override;
    bool Flush() override;
    ~FileSkin() override;
private:
    std::string _filePath;
    std::ofstream _ofs;
    std::mutex _mutex;
};

// 滚动文件输出
class RollingFileSkin : public LogSkin{
public:
    RollingFileSkin(
        const std::string& basePath,
        size_t maxFileSize,
        size_t maxFileNum,
        bool isOpenNewFile
    );
    
    void Skin(const std::string& formattedLog) override;
    bool Flush() override;
    ~RollingFileSkin() override;
private:
    std::mutex _mutex;
    std::ofstream _ofs;
    std::string _basePath;
    size_t _maxFileSize;
    size_t _maxFileNum;
    size_t _currentSize = 0;

    void RollFile();
};