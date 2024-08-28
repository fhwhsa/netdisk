#ifndef _CONN_RESOURCES_H_
#define _CONN_RESOURCES_H_

#include <unordered_map>
#include <fstream>

/// @brief 用户系统资源
class ConnResources
{
public:
    ConnResources();
    ConnResources(std::string _userId);
    ~ConnResources();

    std::string getUserId();
    void setUserId(std::string _userId);

    std::string getFilePath();
    void setFilePath(std::string _filepath);

    void setFd(int _fd);
    int getFd();

private:
    /// @brief 用户id
    std::string userId;
    /// @brief 上传/下载文件时用于保存文件路径
    std::string filepath;
    /// @brief 上传/下载文件时用于保存打开的文件描述符，避免频繁打开/关闭文件
    int fd;
};

#endif