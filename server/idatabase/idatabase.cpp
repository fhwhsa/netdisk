#include "idatabase.h"
#include "../mysql/mysqlConn.h"
#include "../mysql/connectionPool.h"

#include <memory>
#include <iostream>
#include <algorithm>
#include <cctype>

void IDatabase::serverOffline()
{
    using namespace std;
    string sql("update user set status =0;");
    ConnectionPool* pool = ConnectionPool::getConnectionPool();
    shared_ptr<MysqlConn> ptr = pool->getConnection(); 
    if (nullptr != ptr)
        ptr->update(sql);
}

int IDatabase::authentication(std::string email, std::string passwd, std::string &handleInfo)
{
    using namespace std;
    string sql("select * from user where email='" + email + "';");
    ConnectionPool* pool = ConnectionPool::getConnectionPool();
    do
    {
        shared_ptr<MysqlConn> ptr = pool->getConnection();
        if (nullptr == ptr || !ptr->query(sql))
        {
            handleInfo = "There was an error with the database interaction.";
            break;
        }

        if (!ptr->next())
        {
            handleInfo = "The user does not exist.";
            break;
        }
        
        try
        {
            if (passwd != ptr->value(2))
            {
                handleInfo = "Wrong password.";
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
            handleInfo = "The user is online.";
            break;
        }
        
        // 更改用户在线状态
        sql = "update user set status=1 where id=" + id + ";";
        if (!ptr->update(sql))
        {
            handleInfo = "There was an error with the database interaction";
            break;
        }

        handleInfo = "";
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

std::pair<std::string, std::string> IDatabase::searchUser(std::string key, std::string &handleInfo)
{
    using namespace std;
    string sql;
    if (all_of(key.begin(), key.end(), ::isdigit))
        sql = string("select id, email from user where id = " + key + ";");
    else 
        sql = string("select id, email from user where email = '" + key + "';");

    // cout << sql << endl;
    ConnectionPool* pool = ConnectionPool::getConnectionPool();
    do
    {
        shared_ptr<MysqlConn> ptr = pool->getConnection();
        if (nullptr == ptr || !ptr->query(sql))
        {
            handleInfo = "There was an error with the database interaction";
            break;
        }

        if (!ptr->next())
        {
            handleInfo = "The user does not exist.";
            break;
        }

        // 获取用户id及邮箱
        string id, email;
        try
        {
            id = ptr->value(0);
            email = ptr->value(1);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            handleInfo = e.what();
            break;
        }

        return make_pair(id, email);
    } while (0);

    return make_pair("-1", "");
}

int IDatabase::addFriendApplication(std::string from, std::string to, std::string &handleinfo)
{
    using namespace std;

    ConnectionPool* pool = ConnectionPool::getConnectionPool();
    do
    {
        shared_ptr<MysqlConn> sptr = pool->getConnection();
        if (nullptr == sptr)
            break;

    // 是否已经为好友，由客户端检查

    // 检查对方是否已提出申请
    if (!sptr->query(string("select uid from friendApplication where uid = '" + to + "' and tid = '" + from + "' and status = 0;")))
        break;
    else if (sptr->next())
    {
        handleinfo = "The other person has initiated a friend request.";
        return 0;
    }

    // 检查是否为重复的申请
    if (!sptr->query(string("select uid from friendApplication where uid = '" + from + "' and tid = '" + to + "' and status = 0;")))
        break;
    else if (sptr->next())
    {
        handleinfo = "Duplicate friend requests.";
        return 0;
    }

    // 添加申请记录
    if (!sptr->update(string("insert into friendApplication (uid, tid, status) values ('" + from + "', '" + to + "', 0);")))
        break;
    else 
        return 1;
    
    } while (0);
    
    handleinfo = "There was an error with the database interaction";
    return -1;
}

std::vector<std::string> IDatabase::getFriendApplicationList(std::string from, std::string& handleinfo, bool res)
{
    using namespace std;

    ConnectionPool* pool = ConnectionPool::getConnectionPool();
    do
    {
        shared_ptr<MysqlConn> sptr = pool->getConnection();
        if (nullptr == sptr)
            break;

        vector<string> data;
        // 查找好友申请（别的用户发起）
        string sql = "select uid, email, fa.status from friendApplication fa, user u where tid = " + from + " and fa.uid = u.id;";
        cout << sql << endl;
        if (!sptr->query(sql))
            break;
        try
        {
            while (sptr->next())
                data.emplace_back(sptr->value(0) + ":" + sptr->value(1) + ":" + to_string(stoi(sptr->value(2)) + 3) + "\r\n");
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            handleinfo = string(e.what());
            break;
        }
        
        // 查找自己发起的申请
        sql = "select tid, email, fa.status from friendApplication fa, user u where uid = " + from + " and fa.tid = u.id;";
        cout << sql << endl;
        if (!sptr->query(sql))
            break;
        try
        {
            while (sptr->next())
                data.emplace_back(sptr->value(0) + ":" + sptr->value(1) + ":" + sptr->value(2) + "\r\n");
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            handleinfo = string(e.what());
            break;
        }
        
        res = true;
        return data;
    } while (0);
    
    res = false;
    handleinfo = "There was an error with the database interaction";
    return {};
}

bool IDatabase::friendVerification(std::string from, std::string to, int flag, std::string& handleinfo)
{
    using namespace std;

    string sql = "update friendApplication set status = " + to_string(flag) + " where uid = " + from + " and tid = " + to + ";";
    cout << sql << endl;

    // ConnectionPool* pool = ConnectionPool::getConnectionPool();
    // do
    // {
    //     shared_ptr<MysqlConn> sptr = pool->getConnection();
    //     if (nullptr == sptr)
    //         break;

    //     if (!sptr->update(sql))    
    //         break;

    //     return true;
    // } while (0);
    
    // handleinfo = "There was an error with the database interaction";
    return false;
}
