cmake_minimum_required(VERSION 3.14)
project(AsynLogger VERSION 1.0.0 LANGUAGES CXX)

# 全局C++标准配置 保底C++11，自动兼容17/20特性分支
# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 默认编译类型，避免Linux默认编译混乱
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# ===================== 核心跨平台兼容修复 =====================
# 解决Linux GCC/Clang C++17 std::filesystem 链接报错问题
# Windows MSVC 不需要额外链接，GCC/Clang必须加文件系统库
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    link_libraries(stdc++fs)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    link_libraries(c++fs)
endif()

# ===================== 编译静态日志库 =====================
# 自动收集src下所有源码，打包静态库
file(GLOB_RECURSE ASYNLOGGER_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_library(AsynLogger STATIC ${ASYNLOGGER_SOURCES})

# 公开头文件路径，外部程序直接 #include <AsynLogger/xxx.hpp>
target_include_directories(AsynLogger
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

# ===================== 示例程序 example/demo.cpp =====================
add_executable(demo example/demo.cpp)
target_link_libraries(demo PRIVATE AsynLogger)

# ===================== 单元测试 test目录 =====================
enable_testing()
file(GLOB_RECURSE TEST_SOURCES ${PROJECT_SOURCE_DIR}/test/*.cpp)
add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests PRIVATE AsynLogger)
add_test(NAME AsynLoggerTests COMMAND tests)