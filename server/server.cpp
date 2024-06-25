/**
 * 服务器
*/

#include "server.h"
#include "wrap/wrap.h"
#include "msg/msgUnit.h"
#include "msg/msgParsing.h"
#include "idatabase/idatabase.h"

#include <stdlib.h>
#include <signal.h>
#include <event.h>
#include <event2/listener.h>
#include <iostream>

struct timeval connSustainTime;

int run(std::string host, uint port, struct timeval _connSustainTime)
{
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
    if (signal(SIGPIPE, SIG_IGN))
        perr_exit("signal error");

    event_base_dispatch(ebase);

    evconnlistener_free(listener);
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

    // 设置读取水位
    bufferevent_setwatermark(bev, EV_READ, sizeof(struct MsgUnit), 0);

    my_bev* mbev = new my_bev();
    mbev->bev = bev;
    mbev->loginId = "";

    bufferevent_set_timeouts(bev, &connSustainTime, nullptr);
    bufferevent_setcb(bev, read_cb, NULL, event_cb, mbev);
    bufferevent_enable(bev, EV_READ | EV_WRITE);
}

void read_cb(struct bufferevent *bev, void *ctx)
{
    // Printf("read\n");
    int res = 0;
    my_bev* mbev = (my_bev*)ctx;

    // 读取总大小
    uint totalLen;
    res = bufferevent_read(bev, &totalLen, sizeof(uint)); 
    MsgUnit* munit = (MsgUnit*)malloc(totalLen);
    munit->totalLen = totalLen;
    res = bufferevent_read(bev, &munit->msgType, sizeof(uint));
    res = bufferevent_read(bev, &munit->msgLen, sizeof(uint));
    res = bufferevent_read(bev, munit->msg, munit->msgLen);

    MsgUnit* respond = MsgParsing::parsing(munit);
    std::cout << respond->totalLen << "," << respond->msgType << "," << respond->msgLen << ","
            << (char*)respond->msg << std::endl;

    if (nullptr != respond)
    {
        if (MsgType::MSG_TYPE_LOGIN_RESPOND == respond->msgType)
            mbev->loginId = MsgParsing::getRow(respond, 1).substr(3);
        bufferevent_write(bev, (char*)respond, respond->totalLen);
    }
    // std::cout << "write" << std::endl;
}

void event_cb(struct bufferevent *bev, short what, void *ctx)
{
    if (what & BEV_EVENT_EOF || what & BEV_EVENT_TIMEOUT)
    {
        my_bev* mbev = (my_bev*)ctx;
        if ("" != mbev->loginId)
            IDatabase::logout(mbev->loginId);
        if (what & BEV_EVENT_EOF)
            Printf("Connection closed\n");
        else 
            Printf("Connection timeout\n");
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
}