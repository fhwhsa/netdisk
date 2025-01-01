#include "connectionPool.h"

std::atomic<int> stopFlag; // 作为两个分离线程的桥梁使用

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

ConnectionPool& ConnectionPool::getInstance()
{
    static ConnectionPool pool;
    return pool;
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

void ConnectionPool::init(std::string _ip, uint _port, std::string _userName, std::string _passwd, std::string _dbName, uint _minConnSize, uint _maxConnSize, ulong _maxIdleTime, ulong _maxWaitTime)
{
    ip = _ip;
    port = _port;
    userName = _userName;
    passwd = _passwd;
    dbName = _dbName;
    minConnSize = _minConnSize;
    maxConnSize = _maxConnSize;
    maxIdleTime = _maxIdleTime;
    maxWaitTime = _maxWaitTime;

    createConnection();
    LogFunc::info("The database connection pool is initialized.");
}

void ConnectionPool::initFromConfig(const Json::Value &root)
{
    if (!root.isMember("connection-pool"))
        throw std::runtime_error("Profile is missing value 'connection-pool'!");
    
    auto check = [&](const std::string& key) {
        if (!root["connection-pool"].isMember(key))
            throw std::runtime_error("The connection-pool configuration is missing the value '" + key + "'");
    };
    
    for (const std::string& key : {"ip", "port", "userName", "passwd", "dbName", 
        "minConnSize", "maxConnSize", "maxIdleTime", "maxWaitTime"})
        check(key);
    
    this->ip = root["connection-pool"]["ip"].asString();
    this->port = root["connection-pool"]["port"].asUInt();
    this->userName = root["connection-pool"]["userName"].asString();
    this->passwd = root["connection-pool"]["passwd"].asString();
    this->dbName = root["connection-pool"]["dbName"].asString();
    this->minConnSize = root["connection-pool"]["minConnSize"].asUInt();
    this->maxConnSize = root["connection-pool"]["maxConnSize"].asUInt();
    this->maxIdleTime = root["connection-pool"]["maxIdleTime"].asUInt64();
    this->maxWaitTime = root["connection-pool"]["maxWaitTime"].asUInt64();

    createConnection();
    LogFunc::info("The database connection pool initialization is complete.");
}

void ConnectionPool::close()
{
    LogFunc::info("Start cleaning up database connection pool resources.");
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
    LogFunc::info("Database connection pool  resource cleanup is complete.");
}

ConnectionPool::~ConnectionPool()
{
    if (-1 == stopFlag.load())
        close();
}

ConnectionPool::ConnectionPool()
{
    ip = "";
    port = 0;
    userName = "";
    passwd = "";
    dbName = "";
    minConnSize = 0;
    maxConnSize = 0;
    maxIdleTime = 0;
    maxWaitTime = 0;
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
        LogFunc::info("Database connection pool has adjust the number of connection to %ld", totalConnSize);

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
                LogFunc::info("Database connection pool has adjust the number of connection to %ld", totalConnSize);
            }
        }
    }
    stopFlag++;
}

void ConnectionPool::createConnection()
{
    assert(ip != "");
    assert(port > 0);
    assert(userName != "");
    assert(passwd != "");
    assert(dbName != "");
    assert(minConnSize > 0);
    assert(maxConnSize > 0);
    assert(maxIdleTime > 0);
    assert(maxWaitTime > 0);

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
    for (uint i = 1; i < minConnSize; ++i)
        addConnection();

    // 设置线程终止标志为否
    stopFlag.store(-1);

    std::thread producer(&ConnectionPool::produceConnection, this);
    std::thread recycler(&ConnectionPool::recycleConnection, this);

    producer.detach();
    recycler.detach();
}
