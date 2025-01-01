#ifndef _BUFFER_H_
#define _BUFFER_H_

#define SCALEIN_THRESHOLD 10 // 缓冲区收缩阈值

#include <string>
#include <vector>
#include <atomic>
#include <functional>
#include <memory>
#include <algorithm>
#include <unistd.h>
#include <sys/uio.h>
#include <string.h>

class Buffer 
{
public:

    /// @brief 
    /// @param initBuffSize 初始缓冲大小
    Buffer(int initBuffSize = 1024);

    /// @brief 设置缓冲区可读数据大于等于一定值时触发回调函数
    /// @param th 回调函数触发门槛
    /// @param cb 回调函数
    /// @param iniBuffSize 初始缓冲大小
    Buffer(size_t th, std::function<void()> cb, int iniBuffSize = 1024);
 
    ~Buffer() = default;

    /// @brief 返回缓冲区尾部可写入的字节数
    /// @return 
    size_t writableBytes() const;       
    /// @brief 返回缓冲区可读取的字节数
    /// @return 
    size_t readableBytes() const ;
    /// @brief 返回缓冲区首部可写入的字节数
    /// @return 
    size_t prependableBytes() const;

    /// @brief 返回当前可读数据的指针
    /// @return 
    const char* peek() const;
    /// @brief 确保缓冲区有足够的空间写入len个字节
    /// @param len 
    void ensureWriteable(size_t len);

    /// @brief 移除指定大小的可读数据
    /// @param len 
    /// @return 操作结果
    bool retrieve(size_t len);
    /// @brief 移除可读数据直到指定的结束指针。
    /// @param end 
    /// @return 操作结果
    bool retrieveUntil(const char* end);

    /// @brief 移除所有可读数据
    void retrieveAll() ;
    /// @brief 移除所有可读数据并转换为字符串返回
    /// @return 可读数据串
    std::string retrieveAllToStr();

    /// @brief 返回缓冲区首部可写入的指针
    /// @return 
    char* beginWrite();
    const char* beginWriteConst() const;

    void append(const std::string& str);
    void append(const char* str, size_t len);
    void append(const void* data, size_t len);
    void append(const Buffer& buff);

    /// @brief 从指定的文件描述符读取数据到缓冲区
    /// @param fd 指定文件描述符
    /// @param eno 错误码
    /// @return 读取字节数
    ssize_t readFd(int fd, int& eno);
    /// @brief 将缓冲区中的数据写入指定的文件描述符
    /// @param fd 指定文件描述符
    /// @param eno 错误码
    /// @return 写入字节数
    ssize_t writeFd(int fd, int& eno);

    /// @brief 设置缓冲区可读数据大于等于一定值时触发回调函数
    /// @param th 回调函数触发门槛
    /// @param cb 回调函数
    void setCallback(size_t th, std::function<void()> cb);

private:
    /// @brief 返回缓冲区首部指针
    /// @return 
    char* beginPtr();
    const char* beginPtr() const;

    /// @brief 调整缓冲区空间到可容纳len个字节。如果缓冲区首部和尾部空闲段足够则调整可读数据到首部，否则扩展缓冲区
    /// @param len 目标容纳字节数
    void adjustSpace(size_t len);

    /// @brief 检查门槛值，符合条件触发回调函数
    void checkThreshold();

    std::vector<char> buffer;
    std::atomic<std::size_t> readPos;
    std::atomic<std::size_t> writePos;

    size_t threshold;
    std::unique_ptr<std::function<void()>> callback; // 只在调用append时可能触发

    int cnt; // 记录连续检查到vector容量过大（大小的两倍）的次数，达到收缩阈值则尝试减少vector容量
}; 

#endif