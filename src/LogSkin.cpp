#include "AsynLogger/LogSkin.hpp"

//LogSkin
void LogSkin::ThrowOpenError(const std::string& filePath) const {
#if __cplusplus >= 202002L
        throw std::runtime_error(std::format("Failed to open log file: {}", filePath));
#else
        throw std::runtime_error("Failed to open log file: " + filePath);
#endif
}

// class ConsoleSkin
void ConsoleSkin::Skin(const std::string& formattedLog) {
    std::lock_guard<std::mutex> lock(_mutex);
    std::cout << formattedLog << std::endl;
}
bool ConsoleSkin::Flush(){
    std::lock_guard<std::mutex> lock(_mutex);
    std::cout.flush();
    return true;
}


//class FileSkin
#if __cplusplus >= 201703L
    FileSkin::FileSkin(std::string_view filePath) : _filePath(filePath)
    {
        _ofs.open(_filePath, std::ios::out | std::ios::app);
        if(!_ofs.is_open()){
            ThrowOpenError(_filePath);
        }
    }
#elif __cplusplus >= 201103L
    FileSkin::FileSkin(const std::string& filePath) : _filePath(filePath)
    {
        _ofs.open(_filePath, std::ios::out | std::ios::app);
        if(!_ofs.is_open()){
            ThrowOpenError(_filePath);
        }
    }
#endif

void FileSkin::Skin(const std::string& formattedLog) {
    std::lock_guard<std::mutex> lock(_mutex);
    _ofs << formattedLog << "\n";
}

bool FileSkin::Flush() {
    std::lock_guard<std::mutex> lock(_mutex);
    _ofs.flush();
    return _ofs.good();
}

FileSkin::~FileSkin() {
    if(_ofs.is_open()){
        _ofs.flush();
        _ofs.close();
    }
}


// class RollingFileSkin
RollingFileSkin::RollingFileSkin(
    const std::string& basePath,
    size_t maxFileSize, 
    size_t maxFileNum,
    bool isOpenNewFile
): _basePath(basePath), _maxFileSize(maxFileSize), _maxFileNum(maxFileNum)
{
    if(isOpenNewFile){
        RollFile();
    }
}

void RollingFileSkin::Skin(const std::string& formattedLog){
    std::lock_guard<std::mutex> lock(_mutex);
    // 是否要滚动
    if(_currentSize + formattedLog.size() > _maxFileSize){
        RollFile();
    }
    _ofs << formattedLog << "\n";
    _currentSize += formattedLog.size();
}

bool RollingFileSkin::Flush(){
    std::lock_guard<std::mutex> lock(_mutex);
    if(_ofs.is_open()){
        _ofs.flush();
    }else{
        return false;
    }
    return _ofs.good();
}

RollingFileSkin::~RollingFileSkin(){
    if(_ofs.is_open()){
        _ofs.flush();
        _ofs.close();
    }
}

#if __cplusplus >= 201703L
void RollingFileSkin::RollFile(){
    if(_ofs.is_open()){
        _ofs.flush();
        _ofs.close();
    }
    // 存在末尾文件就删除
    std::string lastFile = _basePath + "." + std::to_string(_maxFileNum);
    if(std::filesystem::exists(lastFile)){
        std::filesystem::remove(lastFile);
    }
    // 删除后开始滚动
    for(int i = _maxFileNum - 1; i > 0; i--){
        std::string newFile = _basePath + "." + std::to_string(i + 1);
        std::string oldFile = _basePath + "." + std::to_string(i);
        if(std::filesystem::exists(oldFile)){
            std::filesystem::rename(oldFile, newFile);
        }
    }
    // 当前文件改为1
    std::string FirstFile = _basePath + "." + "1";
    if(std::filesystem::exists(_basePath)){
        std::filesystem::rename(_basePath, FirstFile);
    }
    // 创建新文件
    _ofs.open(_basePath, std::ios::out | std::ios::trunc);
    if(!_ofs.is_open()){
        ThrowOpenError(_basePath);
    }
    _currentSize = 0;
}
#else
void RollingFileSkin::RollFile(){
    if(_ofs.is_open()){
        _ofs.flush();
        _ofs.close();
    }
    // 存在末尾文件就删除
    std::string lastFile = _basePath + "." + std::to_string(_maxFileNum);
    struct stat fileInfo;
    if(stat(lastFile.c_str(), &fileInfo) == 0){
        std::remove(lastFile.c_str());
    }
    // 删除后开始滚动
    for(int i = _maxFileNum - 1; i > 0; i--){
        std::string newFile = _basePath + "." + std::to_string(i + 1);
        std::string oldFile = _basePath + "." + std::to_string(i);
        if(stat(oldFile.c_str(), &fileInfo) == 0){
            std::rename(oldFile.c_str(), newFile.c_str());
        }
    }
    // 当前文件改为1
    std::string FirstFile = _basePath + "." + "1";
    if(stat(_basePath.c_str(), &fileInfo) == 0){
        std::rename(_basePath.c_str(), FirstFile.c_str());
    }
    // 创建新文件
    _ofs.open(_basePath, std::ios::out | std::ios::trunc);
    if(!_ofs.is_open()){
        ThrowOpenError(_basePath);
    }
    _currentSize = 0;
}
#endif