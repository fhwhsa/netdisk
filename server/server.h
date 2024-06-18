/**
 * 服务器
 */

#ifndef _SERVER_H_
#define _SERVER_H_

#include <string>
#include <event.h>

/**
 * 描述：运行服务器
 * 返回值：
 *      失败    函数立即结束，返回1
 *      成功    函数进入循环监听状态
 */
int run(std::string host, uint port);

/**
 * 描述：监听套接字回调函数
 */
void listener_cb(struct evconnlistener *, evutil_socket_t, struct sockaddr *, int socklen, void *);

/**
 * 描述：连接读回调
 */
void read_cb(struct bufferevent *, void *);

/**
 * 描述：连接事件回调
 */
void event_cb(struct bufferevent *, short, void *);

/**
 * 描述：信号SIGINT监听回调函数
 */
void sigint_cb(evutil_socket_t, short, void *);

#endif