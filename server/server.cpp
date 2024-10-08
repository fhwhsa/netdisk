/**
 * 服务器
*/

#include "server.h"
#include "wrap/wrap.h"
#include "msg/msgUnit.h"
#include "msg/msgParsing.h"
#include "idatabase/idatabase.h"
#include "mysql/connectionPool.h"
#include "threadpool/threadpool.h"

#include <stdlib.h>
#include <signal.h>
#include <event.h>
#include <event2/listener.h>
#include <iostream>
#include <thread>
#include <arpa/inet.h>

struct timeval connSustainTime;

int run(std::string host, uint port, struct timeval _connSustainTime)
{
    // 初始化数据库连接池
    ConnectionPool::getConnectionPool();

    // 初始化线程池
    ThreadPool::getInstance();

    connSustainTime = _connSustainTime;

    struct event_base *ebase = event_base_new();
    if (!ebase)
    {
        fprintf(stderr, "Could not initialize libevent!\n");
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(host.c_str());

    // 开启端口复用，释放连接监听器会关闭底层套接字
    struct evconnlistener *listener = evconnlistener_new_bind(ebase, listener_cb, ebase,
        LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, -1, (struct sockaddr*)&addr, sizeof(addr));
    if (!listener)
    {
        fprintf(stderr, "Could not initialize libevent!\n");
        return 1;
    }

    // 添加SIGINT信号的监听
    struct event *signalev = evsignal_new(ebase, SIGINT, sigint_cb, ebase);
    if (!signalev || event_add(signalev, NULL) < 0)
    {
        fprintf(stderr, "Could not create/add a signal event!\n");
        return 1;
    }

    // 忽略SIGPIPE信号，避免可能的写操作错误导致服务器退出
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
        perr_exit("signal error");

    event_base_dispatch(ebase);

    evconnlistener_free(listener);
    event_free(signalev);
    event_base_free(ebase);

    return 0;
}

void listener_cb(struct evconnlistener *evlistener, evutil_socket_t fd, struct sockaddr *addr, int socklen, void *arg)
{
    struct event_base* ebase = (struct event_base*)arg;

    struct sockaddr_in* saddr = (struct sockaddr_in*)addr;
    Printf("The new conneciton %s:%d\n", inet_ntoa(saddr->sin_addr), ntohs(saddr->sin_port));
    
    // 释放 bufferevent 时关闭底层传输端口。这将关闭底层套接字,释放底层 bufferevent 等。
    struct bufferevent* bev = bufferevent_socket_new(ebase, fd, BEV_OPT_CLOSE_ON_FREE);
    if (!bev)
    {
        fprintf(stderr, "Error create bufferevent!\n");
        return ;
    }

    // 设置读取低水位为消息的固定字节数
    bufferevent_setwatermark(bev, EV_READ, sizeof(struct MsgUnit), 0);

    my_bev* mbev = new my_bev();
    mbev->bev = bev;
    mbev->isRunningReadCb = false;

    bufferevent_set_timeouts(bev, &connSustainTime, nullptr);
    bufferevent_setcb(bev, read_cb, NULL, event_cb, mbev);
    bufferevent_enable(bev, EV_READ | EV_WRITE);
}

void respondToClient(my_bev* mbev)
{
    mbev->isRunningReadCb = true;

    uint totalLen;
    struct evbuffer* input = bufferevent_get_input(mbev->bev);

    // 因为设置了读取低水位，可以确保数据可读
    evbuffer_copyout(input, &totalLen, sizeof(uint));
    if (static_cast<ulong>(totalLen) > evbuffer_get_length(input))  // 缓冲区数据不足则退出
    {
        mbev->isRunningReadCb = false;
        return;
    }
    
    MsgUnit* munit = (MsgUnit*)malloc(totalLen);
    evbuffer_remove(input, munit, totalLen);
    mbev->isRunningReadCb = false;

    MsgUnit* respond = MsgParsing::parsing(munit, mbev->ur);
    if (nullptr != respond)
    {
        // std::cout << respond->totalLen << "," << respond->msgType << "," << respond->msgLen << ","
        //         << (char*)respond->msg << std::endl;
        
        mbev->isRunningReadCb = true;
        bufferevent_write(mbev->bev, (char*)respond, respond->totalLen);
        mbev->isRunningReadCb = false;
        try
        {
            free(respond);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        
    }
    // std::cout << "write" << std::endl;
    try
    {
        /* code */
        free(munit);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}

void read_cb(struct bufferevent *bev, void *ctx)
{
    my_bev* mbev = (my_bev*)ctx;
    if (mbev->isRunningReadCb)
        return;
    ThreadPool::getInstance().enqueue(respondToClient, mbev);
}

void event_cb(struct bufferevent *bev, short what, void *ctx)
{
    if (what & BEV_EVENT_EOF || what & BEV_EVENT_TIMEOUT)
    {
        my_bev* mbev = (my_bev*)ctx;
        if ("" != mbev->ur.getUserId())
            IDatabase::logout(mbev->ur.getUserId());
        if (what & BEV_EVENT_EOF)
            Printf("Connection closed\n");
        else 
            Printf("Connection timeout\n");
        if (nullptr != mbev)
        {
            delete mbev;
            mbev = nullptr;
        }
        bufferevent_free(bev);
    }
    else if (what & BEV_EVENT_ERROR)
        Perror("Got an error on conneciton!\n");
    else ;
}

void sigint_cb(evutil_socket_t sig, short events, void *arg)
{
	struct event_base *base = (event_base*)arg;
	struct timeval delay = { 2, 0 };
	printf("Caught an interrupt signal; exiting...\n");
	event_base_loopexit(base, &delay);
    IDatabase::serverOffline();
}