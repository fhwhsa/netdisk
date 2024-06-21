#include "idatabase.h"
#include "../mysql/mysqlConn.h"
#include "../mysql/connectionPool.h"

#include <memory>
#include <iostream>

bool IDatabase::authentication(std::string email, std::string passwd, std::string& handleInfo)
{
    using namespace std;
    string sql("select * from user where email='" + email + "';");
    ConnectionPool* pool = ConnectionPool::getConnectionPool();
    do
    {
        shared_ptr<MysqlConn> ptr = pool->getConnection();
        if (nullptr == ptr || !ptr.get()->query(sql))
        {
            handleInfo = "服务器错误！";
            break;
        }

        if (!ptr.get()->next())
        {
            handleInfo = "用户不存在！";
            break;
        }
        
        if (passwd != ptr.get()->value(2))
        {
            handleInfo = "密码错误！";
            break;
        }
        
        handleInfo = "验证通过！";
        // sql = "update user set status=1 where id=" + ptr.get()->value(0) + ";";
        // ptr.get()->update(sql);
        return true;
    } while (0);

    return false;    
}
