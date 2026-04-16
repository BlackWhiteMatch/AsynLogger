cmake_minimum_required(VERSION 3.14)
project(AsynLogger VERSION 1.0.0 LANGUAGES CXX)

# C++标准配置（兼容C++11/C++20）
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# 收集核心源码
file(GLOB_RECURSE ASYNLOGGER_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# 生成静态库
add_library(AsynLogger STATIC ${ASYNLOGGER_SOURCES})
target_include_directories(AsynLogger PUBLIC ${PROJECT_SOURCE_DIR}/include)

# 示例程序
add_executable(demo example/demo.cpp)
target_link_libraries(demo PRIVATE AsynLogger)

# 单元测试
enable_testing()
file(GLOB_RECURSE TEST_SOURCES ${PROJECT_SOURCE_DIR}/test/*.cpp)
add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests PRIVATE AsynLogger)
add_test(NAME AsynLoggerTests COMMAND tests)