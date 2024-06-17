/**
 * 数据库连接池
 */

#ifndef _CONNECTIONPOOL_H_
#define _CONNECTIONPOOL_H_

#include "mysqlConn.h"
#include <mutex>
#include <condition_variable>
#include <memory>
#include <queue>
#include <chrono>
#include <atomic>

// 继承MysqlConn，添加类成员记录数据库连接变为空闲状态的时间点
class _MysqlConn : public MysqlConn
{
public:
    _MysqlConn();

    /**
     * 描述：获取空闲时间，单位为毫秒
     * 返回值：空闲时间，单位为毫秒
     */
    ulong getIdleTime();

    /**
     * 描述：初始化最近空闲时间点
     */
    void initLastIdlePoint();

private:
    std::chrono::system_clock::time_point lastIdlePoint;     /*当前连接最近一次变为空闲状态的时间点*/
};

class ConnectionPool
{
public:
    /**
     * 描述：获取数据库连接池对象
     * 返回值：数据库连接池对象
     */
    static ConnectionPool* getConnectionPool();
    
    /**
     * 描述：获取数据库连接
     * 返回值：
     *      成功    shared_ptr封装的数据库连接的子类对象指针
     *      失败    空指针
     */
    std::shared_ptr<_MysqlConn> getConnection();

    ~ConnectionPool();


private:
    // 禁止构造，拷贝构造以及拷贝赋值
    ConnectionPool();
    ConnectionPool(const ConnectionPool& obj);
    ConnectionPool& operator=(const ConnectionPool& obj);

    /**
     * 加载配置文件
     */
    void loadConfig();

    /**
     * 向数据库连接队列添加一个数据库连接
     */
    void addConnection();

    /**
     * 线程函数，用于动态添加数据库连接
     */
    void produceConnection();

    /**
     * 线程函数，用于动态回收数据库连接
     */
    void recycleConnection();

    std::string ip;             /*数据库ip地址*/
    uint port;                  /*数据库端口*/
    std::string userName;       /*登陆用户名*/
    std::string passwd;         /*登陆密码*/
    std::string dbName;         /*数据库名称*/
    uint minConnSize;           /*最小连接个数*/
    uint maxConnSize;           /*最大连接个数*/
    ulong maxIdleTime;          /*连接最大空闲时长*/
    ulong maxWaitTime;          /*获取连接最大等待时长*/

    std::queue<_MysqlConn*> connQueue;          /*数据库连接队列*/
    uint totalConnSize;                         /*总连接数，包含没被取走的和取走的*/
    std::mutex cqmutex;                         /*connQueue和totalConnSize的互斥锁*/
    std::condition_variable cond_put;           /*生产连接条件变量*/
    std::condition_variable cond_take;          /*取出连接条件变量*/
};

#endif