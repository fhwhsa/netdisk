/**
 * 服务器
*/

#include "wrap.h"
#include "dataUnit.h"
#include <stdlib.h>
#include <signal.h>
#include <event.h>
#include <event2/listener.h>
#include "mysql.h"

#define IPADDR "127.0.0.1"
#define PORT 9999

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

int main()
{
    struct event_base *ebase = event_base_new();
    if (!ebase)
    {
        fprintf(stderr, "Could not initialize libevent!\n");
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(IPADDR);

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

    // 设置读取地位
    bufferevent_setwatermark(bev, EV_READ, sizeof(struct DataUnit), 0);

    bufferevent_setcb(bev, read_cb, NULL, event_cb, NULL);
    bufferevent_enable(bev, EV_READ | EV_WRITE);
}

void read_cb(struct bufferevent *bev, void *ctx)
{
    Printf("read\n");

    int res = 0;

    // 读取总大小
    uint totalLen;
    res = bufferevent_read(bev, &totalLen, sizeof(uint)); 
    Printf("%d:%d\n", totalLen, res);

    DataUnit* dunit = (DataUnit*)malloc(totalLen);
    dunit->totalLen = totalLen;

    res = bufferevent_read(bev, &dunit->dataType, sizeof(uint));
    Printf("%d:%d\n", dunit->dataType, res);

    res = bufferevent_read(bev, dunit->fileName, sizeof(dunit->fileName));
    Printf("%s:%d\n", dunit->fileName, res);

    res = bufferevent_read(bev, &dunit->dataLen, sizeof(uint));
    Printf("%d:%d\n", dunit->dataLen, res);

    res = bufferevent_read(bev, dunit->data, dunit->dataLen);
    Printf("%s:%d\n", (char*)dunit->data, res);
    
}

void event_cb(struct bufferevent *bev, short what, void *ctx)
{
    if (what & BEV_EVENT_EOF)
    {
        Printf("Connection closed\n");
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

	printf("Caught an interrupt signal; exiting cleanly in two seconds.\n");

	event_base_loopexit(base, &delay);
}


