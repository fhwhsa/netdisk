/**
 * 数据库API封装
 */

#ifndef _MYSQLCONN_H_
#define _MYSQLCONN_H_

#include <mysql/mysql.h>
#include <string>
#include <unordered_map>
#include <stdexcept>

/// @brief C API封装的数据库连接
class MysqlConn
{
public:
    MysqlConn();

    ~MysqlConn();

    /// @brief 连接数据库
    /// @param host 数据库ip地址
    /// @param user 数据库登陆用户名
    /// @param passwd 数据库登陆用户密码
    /// @param dbname 数据库名称
    /// @param port 目标数据库监听端口
    /// @return 执行成功返回true
    bool connect(std::string host, std::string user, std::string passwd, std::string dbname, unsigned int port = 0);

    /// @brief 更新数据库数据
    /// @param sql sql语句
    /// @return 成功返回true
    bool update(std::string sql);

    /// @brief 查询数据库
    /// @param sql sql语句
    /// @return 成功返回true
    bool query(std::string sql);

    /// @brief 将游标指向结果集下一条记录
    /// @return 结果集有下一条记录返回true
    bool next();

    /// @brief 检查结果集是否为空
    /// @return 结果集不为空返回true
    bool hasRes();

    /// @brief 获取结果集列数
    /// @return 结果集列数
    unsigned int getColNum();

    /// @brief 获取结果中的字段值
    /// @param index 根据下标取值
    /// @exception 下标越界抛出异常
    /// @return 对应字段值
    std::string value(unsigned int index);

    /// @brief 获取结果中的字段值
    /// @param index 根据字段名取值
    /// @exception 没有字段名抛出异常
    /// @return 对应字段值
    std::string value(std::string item);

    /// @brief 开启事务
    /// @return 成功返回true
    bool start_transaction();

    /// @brief 提交事务
    /// @return 成功返回true
    bool commit();

    /// @brief 事务回滚
    /// @return 成功返回true
    bool rollback();


private:
    /// @brief 数据库连接
    MYSQL *mysqlconn = nullptr;

    /// @brief 结果集
    MYSQL_RES *mysqlres = nullptr;
    /// @brief 结果集列的字段定义
    MYSQL_FIELD *mysqlfield = nullptr;
    /// @brief 结果集列数
    unsigned int num = 0;
    /// @brief 当前行各值长度
    unsigned long *len = nullptr;
    /// @brief 当前行
    MYSQL_ROW mysqlrow = nullptr;
    /// @brief 字段值对应下标
    std::unordered_map<std::string, int> stoindex;
    /// @brief 进行查询操作前初始化数据
    void initQuery();
};

#endif