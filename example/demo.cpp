#include <iostream>
#include <AsynLogger/LogSkin.hpp>
#include <sys/stat.h>
#include <string>

// 所有演示日志统一存放目录
const std::string DEMO_LOG_DIR = "demo_logs";

// 跨平台自动创建日志文件夹 Windows/Linux通用
void CreateDemoLogFolder()
{
#ifdef _WIN32
    _mkdir(DEMO_LOG_DIR.c_str());
#else
    mkdir(DEMO_LOG_DIR.c_str(), 0755);
#endif
}

int main()
{
    CreateDemoLogFolder();

    std::cout << "AsynLogger 异步日志库示例程序" << std::endl;
    std::cout << "当前编译C++标准: " << __cplusplus << std::endl << std::endl;

    // 1. 控制台日志皮肤测试
    std::cout << "1. 控制台日志输出演示" << std::endl;
    ConsoleSkin console;
    console.Skin("[INFO] 程序正常运行");
    console.Skin("[WARN] 控制台日志输出正常");
    console.Flush();
    std::cout << std::endl;

    // 2. 普通追加文件日志
    std::cout << "2. 固定单文件日志演示" << std::endl;
    std::string normalPath = DEMO_LOG_DIR + "/normal_log.log";
    try
    {
        FileSkin fileSkin(normalPath);
        fileSkin.Skin("[INFO] 普通文件日志内容1");
        fileSkin.Skin("[ERROR] 普通文件错误日志内容2");
        fileSkin.Flush();
        std::cout << "固定文件日志写入完成" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "文件异常: " << e.what() << std::endl;
    }
    std::cout << std::endl;

    // 3. 按大小自动滚动归档日志
    std::cout << "3. 大小触发滚动日志演示" << std::endl;
    std::string rollPath = DEMO_LOG_DIR + "/roll_log.log";
    try
    {
        // 参数：基础路径 | 单文件最大字节 | 最大保留历史文件 | 启动新建文件
        RollingFileSkin rollSkin(rollPath, 180, 3, true);

        // 循环写入快速触发滚动轮转
        for (int i = 1; i <= 12; ++i)
        {
            std::string log = "[INFO] 滚动日志测试序号：" + std::to_string(i);
            rollSkin.Skin(log);
        }

        rollSkin.Flush();
        std::cout << "日志已满自动滚动备份完成" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "滚动日志异常: " << e.what() << std::endl;
    }

    std::cout << std::endl << "所有示例演示结束" << std::endl;
    return 0;
}