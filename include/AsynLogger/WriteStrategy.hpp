#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <thread>
#include <atomic>
#include <AsynLogger/LogSink.hpp>
#include <condition_variable>
#if __cplusplus >= 202002L
#include <format>
#elif __cplusplus >= 201703L
#else
#endif

class WriteStrategy {
public:
    explicit WriteStrategy(const std::vector<std::unique_ptr<LogSink>>& sinks)
        : _sinks(sinks){};
    virtual ~WriteStrategy() = default;
    virtual void Write(const std::string& formattedLog) = 0;
    virtual void Flush() = 0;
    virtual void Start();
    virtual void Stop();
protected:
    const std::vector<std::unique_ptr<LogSink>>& _sinks;
};

// 同步写入策略，直接主线程堵塞写入
class SyncWriteStrategy : public WriteStrategy{
public:
    using WriteStrategy::WriteStrategy;
    void Write(const std::string& formattedLog) override;
    void Flush() override;
};

// 异步写入策略
class AsyncWriteStrategy : public WriteStrategy{
public:
    explicit AsyncWriteStrategy(
        const std::vector<std::unique_ptr<LogSink>>& sinks,
        size_t maxQueueSize = 10240
    ) : WriteStrategy(sinks), _maxQueueSize(maxQueueSize){}
    ~AsyncWriteStrategy() { Stop(); }
    void Write(const std::string& formattedLog) override;
    void Flush() override;
    void Start() override;
    void Stop() override;
private:
    void workLoop();
    size_t _maxQueueSize;
    std::queue<std::string> _logQueue;
    std::condition_variable _cv;
    std::mutex _mutex;
    std::thread _workThread;
    std::atomic_bool _running{false};
};