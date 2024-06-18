#include "connectionPool.h"
#include "../json/json.h"

#include <fstream>
#include <thread>
#include <stdexcept>
// #include <iostream>

std::atomic<int> stopFlag;

_MysqlConn::_MysqlConn()
{
}

ulong _MysqlConn::getIdleTime()
{
    using namespace std;
    using namespace chrono;
    time_point end = system_clock::now();
    std::chrono::milliseconds res = duration_cast<milliseconds>(end - this->lastIdlePoint);
    return (ulong)res.count();
}

void _MysqlConn::initLastIdlePoint()
{
    this->lastIdlePoint = std::chrono::system_clock::now();
}

ConnectionPool* ConnectionPool::getConnectionPool()
{
    static ConnectionPool pool;
    return &pool;
}

std::shared_ptr<_MysqlConn> ConnectionPool::getConnection()
{
    std::unique_lock<std::mutex> locker(cqmutex);
    while (connQueue.empty())
    {
        // 如果超时返回空指针
        if (std::cv_status::timeout == cond_take.wait_for(locker, std::chrono::milliseconds(maxWaitTime)))
            return nullptr;
    }
    
    // 自定义删除器实现连接自动回收
    std::shared_ptr<_MysqlConn> ptr(connQueue.front(), [this](_MysqlConn* conn){
        std::lock_guard<std::mutex> locker(cqmutex);
        conn->initLastIdlePoint();
        connQueue.push(conn);
        // std::cout << "recycle" << std::endl;
    });
    connQueue.pop();
    
    // 唤醒生产连接线程
    locker.unlock();
    cond_put.notify_all();
    
    return ptr;
}

ConnectionPool::~ConnectionPool()
{
    // 关闭生产连接和回收连接这两个线程
    stopFlag.store(0);
    cond_put.notify_all();
    while (stopFlag.load() < 2)
        std::this_thread::sleep_for(std::chrono::seconds(1));
    
    while (!connQueue.empty())
    {
        _MysqlConn* conn = connQueue.front();
        connQueue.pop();
        delete conn;
    }
}

ConnectionPool::ConnectionPool()
{
    // 加载配置文件，创建数据库连接
    loadConfig();

    this->totalConnSize = 0;
    // 先创建一个连接设置Mysql最大连接数
    {
        _MysqlConn* conn = new _MysqlConn();
        if (conn->connect(this->ip, this->userName, this->passwd, this->dbName, this->port))
        {
            std::string sql = "set global max_connections = ";
            sql = sql + std::to_string(this->maxConnSize + 10) + ";";
            conn->update(sql);   
            conn->initLastIdlePoint();
            connQueue.push(conn);
            ++totalConnSize;
        }
        if (0 == totalConnSize)
            throw std::runtime_error("Failed to set the maximum number of MySQL connections!");
    }
    for (int i = 1; i < minConnSize; ++i)
        addConnection();

    // 设置线程终止标志为否
    stopFlag.store(-1);

    std::thread producer(&ConnectionPool::produceConnection, this);
    std::thread recycler(&ConnectionPool::recycleConnection, this);

    producer.detach();
    recycler.detach();
}

void ConnectionPool::loadConfig()
{
    std::ifstream ifs;
    ifs.open("config.json");
    if (!ifs.is_open())
        throw std::runtime_error("Failed to open the configuration file!");

    // utf8支持
    Json::CharReaderBuilder ReaderBuilder;
    ReaderBuilder["emitUTF8"] = true;

    Json::Value root;
    std::string strerr;
    bool ok = Json::parseFromStream(ReaderBuilder, ifs, &root, &strerr);
    if (!ok)
        throw std::runtime_error("Failed to read the configuration file!");

    this->ip = root["ip"].asString();
    this->port = root["port"].asUInt();
    this->userName = root["userName"].asString();
    this->passwd = root["passwd"].asString();
    this->dbName = root["dbName"].asString();
    this->minConnSize = root["minConnSize"].asUInt();
    this->maxConnSize = root["maxConnSize"].asUInt();
    this->maxIdleTime = root["maxIdleTime"].asUInt64();
    this->maxWaitTime = root["maxWaitTime"].asUInt64();
}

void ConnectionPool::addConnection()
{
    _MysqlConn* conn = new _MysqlConn();
    if (conn->connect(this->ip, this->userName, this->passwd, this->dbName, this->port))
    {
        conn->initLastIdlePoint();
        connQueue.push(conn);
        ++totalConnSize;
    }
}

void ConnectionPool::produceConnection()
{
    while (true)
    {
        std::unique_lock<std::mutex> locker(cqmutex);
        while (!(connQueue.size() < minConnSize && totalConnSize < maxConnSize) && -1 == stopFlag.load())
            cond_put.wait(locker);

        if (-1 != stopFlag.load())
            break;

        addConnection();

        // 唤醒取连接线程
        cond_take.notify_all();
    }
    stopFlag++;
}

void ConnectionPool::recycleConnection()
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (-1 != stopFlag.load())
            break;

        std::unique_lock<std::mutex> locker(cqmutex);
        if (connQueue.size() > minConnSize)
        {
            _MysqlConn* conn = connQueue.front();
            if (conn->getIdleTime() >= this->maxIdleTime)
            {
                connQueue.pop();
                --totalConnSize;
                delete conn;
            }
        }
    }
    stopFlag++;
}

