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

/// @brief 继承MysqlConn，添加类成员记录数据库连接变为空闲状态的时间点
class _MysqlConn : public MysqlConn
{
public:
    _MysqlConn();

    /// @brief 获取连接空闲时间，单位为毫秒
    /// @return 空闲时间
    ulong getIdleTime();

    /// @brief 初始化最近空闲时间点
    void initLastIdlePoint();

private:
    /// @brief 当前连接最近一次变为空闲状态的时间点
    std::chrono::system_clock::time_point lastIdlePoint;
};

/// @brief 数据库连接池
class ConnectionPool
{
public:

    /// @brief 获取数据库连接池对象
    /// @return 一个指针指向数据库连接池对象
    static ConnectionPool* getConnectionPool();

    /// @brief 获取数据库连接
    /// @return 成功返回一个共享指针指向_MysqlConn对象
    std::shared_ptr<_MysqlConn> getConnection();

    ~ConnectionPool();


private:
    // 禁止构造，拷贝构造以及拷贝赋值
    ConnectionPool();
    ConnectionPool(const ConnectionPool& obj);
    ConnectionPool& operator=(const ConnectionPool& obj);

    /// @brief 加载配置文件
    void loadConfig();

    /// @brief 向数据库连接队列添加一个数据库连接
    void addConnection();

    /// @brief 线程函数，用于动态添加数据库连接
    void produceConnection();

    /// @brief 线程函数，用于动态回收数据库连接
    void recycleConnection();

    /// @brief 数据库ip地址
    std::string ip;
    /// @brief 数据库端口
    uint port;
    /// @brief 登陆用户名
    std::string userName;
    /// @brief 登陆密码
    std::string passwd;
    /// @brief 数据库名称
    std::string dbName;
    /// @brief 最小连接个数
    uint minConnSize;
    /// @brief 最大连接个数
    uint maxConnSize;
    /// @brief 连接最大空闲时长
    ulong maxIdleTime;
    /// @brief 获取连接最大等待时长
    ulong maxWaitTime;

    /// @brief 数据库连接队列
    std::queue<_MysqlConn*> connQueue;
    /// @brief 总连接数，包含没被取走的和取走的
    uint totalConnSize;
    /// @brief connQueue和totalConnSize的互斥锁
    std::mutex cqmutex;
    /// @brief 生产连接条件变量
    std::condition_variable cond_put;
    /// @brief 取出连接条件变量
    std::condition_variable cond_take;
};

#endif