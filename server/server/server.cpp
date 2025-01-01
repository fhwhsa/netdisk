/**
 * 服务器
 */

#include "server.h"

struct timeval connSustainTime;

int run(std::string host, uint port, const Json::Value &root, struct timeval _connSustainTime)
{
    // 初始化数据库连接池
    ConnectionPool::getInstance().initFromConfig(root);

    // 初始化线程池
    ThreadPool::getInstance().initFromConfig(root);

    connSustainTime = _connSustainTime;

    struct event_base *ebase = event_base_new();
    if (!ebase)
    {
        // fprintf(stderr, "Could not initialize libevent!\n");
        // return 1;
        LogFunc::error("Could not initialize libevent!");
        throw std::exception();
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(host.c_str());

    // 开启端口复用，释放连接监听器会关闭底层套接字
    struct evconnlistener *listener = evconnlistener_new_bind(ebase, listener_cb, ebase,
                                                              LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, -1, (struct sockaddr *)&addr, sizeof(addr));
    if (!listener)
    {
        // fprintf(stderr, "Could not initialize libevent!\n");
        // return 1;
        LogFunc::error("Could not initialize libevent!");
        throw std::exception();
    }

    // 添加SIGINT信号的监听
    struct event *signalev = evsignal_new(ebase, SIGINT, sigint_cb, ebase);
    if (!signalev || event_add(signalev, NULL) < 0)
    {
        // fprintf(stderr, "Could not create/add a signal event!\n");
        // return 1;
        LogFunc::error("Could not create/add a signal event!");
        throw std::exception();
    }

    // 忽略SIGPIPE信号，避免可能的写操作错误导致服务器退出
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    {
        // perr_exit("signal error");
        LogFunc::error("Signal error: %s.", strerror(errno));
    }

    LogFunc::info("Start event loop...");
    Log::getInstance().disableWriteErrorStream();
    if (-1 == event_base_dispatch(ebase))
        LogFunc::error("Failed to start event loop.");

    evconnlistener_free(listener);
    event_free(signalev);
    event_base_free(ebase);

    // 指定销毁顺序
    ThreadPool::getInstance().close();
    ConnectionPool::getInstance().close();

    return 0;
}

void listener_cb(struct evconnlistener *evlistener, evutil_socket_t fd, struct sockaddr *addr, int socklen, void *arg)
{
    struct event_base *ebase = (struct event_base *)arg;

    struct sockaddr_in *saddr = (struct sockaddr_in *)addr;
    // Printf("The new conneciton %s:%d\n", inet_ntoa(saddr->sin_addr), ntohs(saddr->sin_port));
    char *nip = inet_ntoa(saddr->sin_addr);
    int nport = ntohs(saddr->sin_port);
    LogFunc::info("The new conneciton %s:%d.", nip, nport);

    // 释放 bufferevent 时关闭底层传输端口。这将关闭底层套接字,释放底层 bufferevent 等。
    struct bufferevent *bev = bufferevent_socket_new(ebase, fd, BEV_OPT_CLOSE_ON_FREE);
    if (!bev)
    {
        // fprintf(stderr, "Error create bufferevent!\n");
        LogFunc::warn("The new conneciton %s:%d init failed: error create bufferevent.", nip, nport);
        return;
    }

    // 设置读取低水位为消息的固定字节数
    bufferevent_setwatermark(bev, EV_READ, sizeof(struct MsgUnit), 0);

    my_bev *mbev = new my_bev();
    mbev->bev = bev;
    mbev->isRunningReadCb = false;
    mbev->bip = static_cast<std::string>(nip);
    mbev->bport = nport;

    if (-1 == bufferevent_set_timeouts(bev, &connSustainTime, nullptr))
    {
        LogFunc::warn("The new conneciton %s:%d init failed: buffer event set timeouts failed.", nip, nport);
        bufferevent_free(bev);
        delete mbev;
        return;
    }
    bufferevent_setcb(bev, read_cb, NULL, event_cb, mbev);
    if (-1 == bufferevent_enable(bev, EV_READ | EV_WRITE))
    {
        LogFunc::warn("The new conneciton %s:%d init failed: buffer event enable failed", nip, nport);
        bufferevent_free(bev);
        delete mbev;
        return;
    }
}

void respondToClient(my_bev *mbev)
{
    mbev->isRunningReadCb = true;

    uint totalLen;
    struct evbuffer *input = bufferevent_get_input(mbev->bev);

    // 因为设置了读取低水位，可以确保数据可读
    if (-1 == evbuffer_copyout(input, &totalLen, sizeof(uint)))
    {
        LogFunc::warn("Failed to copy out from evbuffer.");
        return;
    }
    if (static_cast<ulong>(totalLen) > evbuffer_get_length(input)) // 缓冲区数据不足则退出
    {
        mbev->isRunningReadCb = false;
        return;
    }

    MsgUnit *munit = (MsgUnit *)malloc(totalLen);
    if (-1 == evbuffer_remove(input, munit, totalLen))
    {
        free(munit);
        LogFunc::warn("Failed to remove from evbuffer.");
        return;
    }
    mbev->isRunningReadCb = false;

    // MsgUnit *respond = MsgParsing::parsing(munit, mbev->ur);
    MsgUnit *respond = MsgParsing::parsing(munit, mbev);
    if (nullptr != respond)
    {
        LogFunc::debug("Respond to %s, totalLen: %ld, msgLen: %ld, type: %d, msg: {%s}.",
                         mbev->getConnectionInfo().c_str(), respond->totalLen, respond->msgLen, respond->msgType, respond->msg);

        mbev->isRunningReadCb = true;
        if (-1 == bufferevent_write(mbev->bev, (char *)respond, respond->totalLen))
        {
            free(munit);
            free(respond);
            return;
        }
        mbev->isRunningReadCb = false;
        free(respond);
    }
    // std::cout << "write" << std::endl;

    free(munit);
}

void read_cb(struct bufferevent *bev, void *ctx)
{
    my_bev *mbev = (my_bev *)ctx;
    LogFunc::debug("Got request from %s", mbev->getConnectionInfo().c_str(), mbev->bport);
    if (mbev->isRunningReadCb)
        return;
    ThreadPool::getInstance().enqueue(respondToClient, mbev);
}

void event_cb(struct bufferevent *bev, short what, void *ctx)
{
    my_bev *mbev = (my_bev *)ctx;
    if (what & BEV_EVENT_EOF || what & BEV_EVENT_TIMEOUT)
    {
        if ("" != mbev->ur.getUserId())
            IDatabase::logout(mbev->ur.getUserId());
        if (what & BEV_EVENT_EOF)
            LogFunc::info("Connection %s closed.", mbev->getConnectionInfo().c_str());
        // Printf("Connection closed\n");
        else
            LogFunc::info("Connection %s timeout.", mbev->getConnectionInfo().c_str());
        // Printf("Connection timeout\n");
        if (nullptr != mbev)
        {
            delete mbev;
            mbev = nullptr;
        }
        bufferevent_free(bev);
    }
    else if (what & BEV_EVENT_ERROR)
        LogFunc::warn("Got as error on connection %s: %s.", mbev->getConnectionInfo().c_str(), strerror(errno));
    // Perror("Got an error on conneciton!\n");
    else
        ;
}

void sigint_cb(evutil_socket_t sig, short events, void *arg)
{
    struct event_base *base = (event_base *)arg;
    struct timeval delay = {2, 0};
    Log::getInstance().enableWriteErrorStream();
    // printf("Caught an interrupt signal; exiting...\n");
    LogFunc::info("Caught an interrupt signal.");
    event_base_loopexit(base, &delay);
    IDatabase::serverOffline();
}