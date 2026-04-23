#include <AsynLogger/WriteStrategy.hpp>
//class WriteStrategy
void WriteStrategy::Start(){}
void WriteStrategy::Stop(){}

// class SyncWriteStrategy
void SyncWriteStrategy::Write(const std::string& formattedLog){
    for(auto& e : _sinks){
        e->Sink(formattedLog);
    }
}

void SyncWriteStrategy::Flush(){
    for(auto& e : _sinks){
        e->Flush();
    }
}


// class AsyncWriteStrategy
void AsyncWriteStrategy::Start(){
    if(_running.load()){
        return;
    }
    _running.store(true);
    _workThread = std::thread(&AsyncWriteStrategy::workLoop, this);
}

void AsyncWriteStrategy::Stop(){
    if(!_running.load()){
        return;
    }
    _running.store(false);
    _cv.notify_all();
    if(_workThread.joinable()){
        _workThread.join();
    }
}

void AsyncWriteStrategy::Write(const std::string& formattedLog){
    if(!_running.load()){
        return;
    }
    std::unique_lock<std::mutex> lock(_mutex);
    _cv.wait(lock, [this]{ 
        return !(_logQueue.size() > _maxQueueSize) || !_running.load();
    });
    if(!_running.load()){
        return;
    }

    _logQueue.push(formattedLog);
    _cv.notify_one();
}

void AsyncWriteStrategy::Flush(){
    {
    std::unique_lock<std::mutex> lock(_mutex);
    _cv.wait(lock, [this]{ return _logQueue.empty();});
    }
    for(auto& e : _sinks){
        e->Flush();
    }
}

void AsyncWriteStrategy::workLoop(){
    while(_running){
        std::queue<std::string> localQueue;
        {
        std::unique_lock<std::mutex> lock(_mutex);
        _cv.wait(lock, [this]{
            return !_logQueue.empty() || !_running.load();
        });
        std::swap(localQueue, _logQueue);
        _cv.notify_all();
        }

        // 写入
        while(!localQueue.empty()){
            const std::string& tmpLogMag = localQueue.front();
            for(auto& e : _sinks){
                e->Sink(tmpLogMag);
            }
            localQueue.pop();
        }
    }
    // 退出前处理剩余的日志
    std::queue<std::string> remainQueue;
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if(!_logQueue.empty()){
            std::swap(remainQueue, _logQueue);
        }
    }
    
    while(!remainQueue.empty()){
        std::string tmpLogMsg = remainQueue.front();
        for(auto& e : _sinks){
            e->Sink(tmpLogMsg);
        }
        remainQueue.pop();
    }

    for(auto& e : _sinks){
        e->Flush();
    }
}