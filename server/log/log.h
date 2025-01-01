#ifndef _LOG_H_
#define _LOG_H_

#include "blockqueue.hpp"
#include "../buffer/buffer.h"
#include "../json/json.h"

#include <string>
#include <mutex>
#include <thread>
#include <memory>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <unistd.h>
#include <atomic>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>

/// @brief 日志等级，从低到高
enum LogLevel
{
    DEBUG = 0,
    INFO,
    WARN,
    ERROR
};

/// @brief 日志模式
enum LogMode
{
    SYNC, // 同步
    ASYNC // 异步
};

class Log
{

public:
    static int BUFFER_SIZE; // 缓冲区参考值  
    static int MAX_LOG_FILE_SIZE; // 单个日志文件参考值
    
    /// @brief 设置日志参数
    /// @param l 有效最低日志等级
    /// @param path 日志文件路径
    /// @param mqs 阻塞队列最大容量，仅在异步模式下有效
    /// @param m 设置日志模式
    void init(LogLevel l, std::string p, LogMode m = ASYNC, int mqs = 1024);

    void initFromConfig(Json::Value& root);

    static Log &getInstance();

    /// @brief 写入日志
    /// @param l 日志等级
    /// @param format 格式字符串
    /// @param
    void writeLog(LogLevel l, const char *format, ...);

    LogLevel getLevel();
    void setLevel(LogLevel l);

    /// @brief 根据当前日期和行号更新日志存储文
    /// @return
    bool updateFile();

    /// @brief 启用日志信息写到标准输出
    void enableWriteErrorStream();
    /// @brief 禁用日志信息写到标准输出
    void disableWriteErrorStream();

private:
    Log();
    virtual ~Log();

    std::string getLogPrefix(LogLevel l);

    static void asyncThreadFunction();
    void asyncWriteLog();

    std::string getCurrDate() const;
    std::string getCurrTime() const;
    std::string getCurrDateAndTime() const;

    // 创建资源
    void createResources();

    // 重置资源
    void resetResources();

    std::string date;

    // 使用指针便于处理异步和同步
    std::unique_ptr<Buffer> buff;
    std::unique_ptr<BlockQueue<std::string>> q;
    std::unique_ptr<std::thread> t;

    LogLevel level;
    std::string basePath;
    int maxQueueSize;
    LogMode mode;

    int fd;
    int fileNo;
    std::mutex mtx;

    std::atomic<bool> hasInit;

    std::atomic<bool> writeErrStream;
};

namespace LogFunc
{
    template <typename... Args>
    void debug(const char *format, Args... args)
    {
        Log::getInstance().writeLog(LogLevel::DEBUG, format, args...);
    }

    template <typename... Args>
    void info(const char *format, Args... args)
    {
        Log::getInstance().writeLog(LogLevel::INFO, format, args...);
    }

    template <typename... Args>
    void warn(const char *format, Args... args)
    {
        Log::getInstance().writeLog(LogLevel::WARN, format, args...);
    }

    template <typename... Args>
    void error(const char *format, Args... args)
    {
        Log::getInstance().writeLog(LogLevel::ERROR, format, args...);
    }
}

#endif //_LOG_H_