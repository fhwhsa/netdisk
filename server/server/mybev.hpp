#ifndef _MYBEV_H_
#define _MYBEV_H_

#include "../connResources/connResources.h"
#include <atomic>

/// @brief 封装bufferevent作为参数传递给回调函数
struct my_bev
{
    struct bufferevent* bev;
    /// @brief 登陆用户的id
    ConnResources ur;
    /// @brief 控制同一连接在同一时刻只有一个线程在执行回调，确保线程安全
    std::atomic<bool> isRunningReadCb;
    /// @brief ip地址
    std::string bip;
    /// @brief 端口
    int bport;

    std::string getConnectionInfo()
    {
        return std::string("<" + bip + ":" + std::to_string(bport) + ":" + ur.getUserId() + ">");
    }
};

#endif