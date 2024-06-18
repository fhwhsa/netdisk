#include "idatabase.h"
#include "../mysql/mysqlConn.h"
#include "../mysql/connectionPool.h"

#include <memory>

bool IDatabase::authentication(std::string email, std::string passwd)
{
    using namespace std;
    string sql("select count(*) from user where email='" + email + "' and passwd='" + passwd + "';");
    ConnectionPool* pool = ConnectionPool::getConnectionPool();
    do
    {
        shared_ptr<MysqlConn> ptr = pool->getConnection();
        if (nullptr == ptr)
            break;

        if (!ptr.get()->query(sql))
            break;
        if (!ptr.get()->next())
            break;

        return "1" == ptr.get()->value(0);
    } while (0);

    return false;    
}