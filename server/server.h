/**
 * 服务器
 */

#ifndef _SERVER_H_
#define _SERVER_H_

#include <string>
#include <event.h>
#include <sys/types.h>
#include <string>

/**
 * 封装bufferevent作为参数传递给回调函数
 */
struct my_bev
{
    struct bufferevent* bev;
    std::string loginId;                                            /*登陆用户的id*/
};

/**
 * 描述：运行服务器
 * 参数：
 *      host                监听ip
 *      port                监听端口
 *      _connSustainTime    连接维持时间
 * 返回值：
 *      失败    函数立即结束，返回1
 *      成功    函数进入循环监听状态
 */
int run(std::string host, uint port, struct timeval _connSustainTime = {60, 0});

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