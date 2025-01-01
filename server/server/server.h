/**
 * 服务器
 */

#ifndef _SERVER_H_
#define _SERVER_H_

#include "../msg/msgUnit.h"
#include "../wrap/wrap.h"
#include "../msg/msgUnit.h"
#include "../msg/msgParsing.h"
#include "../idatabase/idatabase.h"
#include "../mysql/connectionPool.h"
#include "../threadpool/threadpool.hpp"
#include "../log/log.h"
#include "mybev.hpp"

#include <iostream>
#include <thread>
#include <string>
#include <stdlib.h>
#include <signal.h>
#include <event.h>
#include <event2/listener.h>
#include <arpa/inet.h>
#include <event.h>
#include <sys/types.h>

/// @brief 运行服务器
/// @param host 监听ip
/// @param port 监听端口
/// @param _connSustainTime 连接维持时间
/// @param 配置文件的Json::Value对象
/// @return 如果失败函数立即结束返回1，否则函数进入循环监听状态
int run(std::string host, uint port, const Json::Value& root, struct timeval _connSustainTime = {60 * 5, 0});

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