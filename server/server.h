/**
 * 服务器
 */

#ifndef _SERVER_H_
#define _SERVER_H_

#include "connResources/connResources.h"
#include "msg/msgUnit.h"

#include <string>
#include <event.h>
#include <sys/types.h>
#include <string>
#include <iostream>
#include <atomic>

/// @brief 封装bufferevent作为参数传递给回调函数
struct my_bev
{
    struct bufferevent* bev;
    /// @brief 登陆用户的id
    ConnResources ur;
    /// @brief 控制同一连接在同一时刻只有一个线程在执行回调，确保线程安全
    std::atomic<bool> isRunningReadCb;
};

/// @brief 运行服务器
/// @param host 监听ip
/// @param port 监听端口
/// @param _connSustainTime 连接维持时间
/// @return 如果失败函数立即结束返回1，否则函数进入循环监听状态
int run(std::string host, uint port, struct timeval _connSustainTime = {60 * 5, 0});

/// @brief 监听套接字回调函数
void listener_cb(struct evconnlistener *, evutil_socket_t, struct sockaddr *, int socklen, void *);

/// @brief 根据不同的munit作出响应
/// @param mbev 
void respondToClient(my_bev* mbev);

/// @brief 读回调函数
void read_cb(struct bufferevent *, void *);

/// @brief 事件回调函数
void event_cb(struct bufferevent *, short, void *);

/// @brief 信号回调函数
void sigint_cb(evutil_socket_t, short, void *);

#endif