#include "idatabase.h"
#include "../mysql/mysqlConn.h"
#include "../mysql/connectionPool.h"

#include <memory>
#include <iostream>
#include <algorithm>
#include <cctype>

int IDatabase::authentication(std::string email, std::string passwd, std::string& handleInfo)
{
    using namespace std;
    string sql("select * from user where email='" + email + "';");
    ConnectionPool* pool = ConnectionPool::getConnectionPool();
    do
    {
        shared_ptr<MysqlConn> ptr = pool->getConnection();
        if (nullptr == ptr || !ptr->query(sql))
        {
            handleInfo = "服务器错误！";
            break;
        }

        if (!ptr->next())
        {
            handleInfo = "用户不存在！";
            break;
        }
        
        try
        {
            if (passwd != ptr->value(2))
            {
                handleInfo = "密码错误！";
                break;
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            break;
        }
        
        // 获取用户id
        string id;
        try
        {
            id = ptr->value(0);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            break;
        }

        string status;
        // 获取用户在线状态
        try
        {
            status = ptr->value(4);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            break;
        }
        
        if ("1" == status)
        {
            handleInfo = "用户已在线";
            break;
        }
        
        // 更改用户在线状态
        sql = "update user set status=1 where id=" + id + ";";
        if (!ptr->update(sql))
        {
            handleInfo = "服务器错误！";
            break;
        }

        handleInfo = "验证通过！";
        return stoi(id);
    } while (0);

    return -1;    
}

bool IDatabase::logout(std::string id)
{
    using namespace std;
    string sql("update user set status=0 where id=" + id + ";");
    ConnectionPool* pool = ConnectionPool::getConnectionPool();
    return pool->getConnection()->update(sql);
}

std::string IDatabase::searchUser(std::string key, std::string &handleInfo)
{
    using namespace std;

    string sql("select email from user where id='" + key + "' or email='" + key + "';");
    if (all_of(key.begin(), key.end(), ::isdigit))
        sql = string("select email from user where id = " + key + ";");
    else 
        sql = string("select email from user where email = '" + key + "';");

    // cout << sql << endl;
    ConnectionPool* pool = ConnectionPool::getConnectionPool();
    do
    {
        shared_ptr<MysqlConn> ptr = pool->getConnection();
        if (nullptr == ptr || !ptr->query(sql))
        {
            handleInfo = "服务器错误！";
            break;
        }

        if (!ptr->next())
        {
            handleInfo = "用户不存在！";
            break;
        }

        // 获取用户邮箱
        string email;
        try
        {
            email = ptr->value(0);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        handleInfo = "查找成功";

        return email;
    } while (0);

    return "";
}
