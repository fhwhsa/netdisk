#include "idatabase.h"

std::string IDatabase::getDateTime()
{
    using namespace std;
    using namespace chrono;
    auto now = system_clock::now();
    time_t now_time_t = system_clock::to_time_t(now);
    stringstream datetime_stream;
    datetime_stream << put_time(localtime(&now_time_t), "%Y-%m-%d %H:%M:%S");
    return datetime_stream.str();
}

void IDatabase::serverOffline()
{
    using namespace std;
    string sql("update user set status =0;");
    ConnectionPool& pool = ConnectionPool::getInstance();
    shared_ptr<MysqlConn> ptr = pool.getConnection(); 
    if (nullptr != ptr)
        ptr->update(sql);
}

int IDatabase::authentication(std::string email, std::string passwd, int& statusCode)
{
    using namespace std;
    string sql("select * from user where email='" + email + "';");
    ConnectionPool& pool = ConnectionPool::getInstance();
    do
    {
        shared_ptr<MysqlConn> ptr = pool.getConnection();
        if (nullptr == ptr)
        {
            statusCode = DATABASEBUSY;
            break;
        }
        if (!ptr->query(sql))
        {
            statusCode = DATABASEERROR;
            break;
        }

        if (!ptr->next())
        {
            statusCode = USERNOTEXIST;
            break;
        }
        
        try
        {
            if (passwd != ptr->value(2))
            {
                statusCode = WRONGPASSWD;
                break;
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            statusCode = EXCEPTION;
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
            statusCode = EXCEPTION;
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
            statusCode = EXCEPTION;
            break;
        }
        
        if ("1" == status)
        {
            statusCode = USERISONLINE;
            break;
        }
        
        // 更改用户在线状态
        sql = "update user set status=1 where id=" + id + ";";
        if (!ptr->update(sql))
        {
            statusCode = DATABASEERROR;
            break;
        }

        statusCode = SUCCESS;
        return stoi(id);
    } while (0);

    return -1;    
}

bool IDatabase::logout(std::string id)
{
    using namespace std;
    string sql("update user set status=0 where id=" + id + ";");
    ConnectionPool& pool = ConnectionPool::getInstance();
    return pool.getConnection()->update(sql);
}

std::pair<std::string, std::string> IDatabase::searchUser(std::string key, int& statusCode)
{
    using namespace std;
    string sql;
    if (all_of(key.begin(), key.end(), ::isdigit))
        sql = string("select id, email from user where id = " + key + ";");
    else 
        sql = string("select id, email from user where email = '" + key + "';");

    // cout << sql << endl;
    ConnectionPool& pool = ConnectionPool::getInstance();
    do
    {
        shared_ptr<MysqlConn> ptr = pool.getConnection();
        if (nullptr == ptr) 
        {
            statusCode = DATABASEBUSY;
            break;
        }
        if (!ptr->query(sql))
        {
            statusCode = DATABASEERROR;
            break;
        }

        if (!ptr->next())
        {
            statusCode = USERNOTEXIST;
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
            statusCode = EXCEPTION;
            break;
        }

        statusCode = SUCCESS;
        return make_pair(id, email);
    } while (0);

    return make_pair("-1", "");
}

int IDatabase::addFriendApplication(std::string from, std::string to, int& statusCode)
{
    using namespace std;

    ConnectionPool& pool = ConnectionPool::getInstance();
    do
    {
        shared_ptr<MysqlConn> sptr = pool.getConnection();
        if (nullptr == sptr)
        {
            statusCode = DATABASEBUSY;
            break;
        }


        string sql;
        // 是否已经为好友
        sql = "select count(*) cnt from friend where status = 0 and  ((uid_1 = " + from + " and uid_2 = " + to + ") "
            "or (uid_1 = " + to + " and uid_2 = " + from + "));";
        if (!sptr->query(sql))
        {
            statusCode = DATABASEERROR;
            break;
        }
        try
        {
            if (sptr->next() && "0" != sptr->value(0))
            {
                statusCode = FRIENDSHIPS;
                return 0;
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            statusCode = EXCEPTION;
            break;
        }
        

        // 检查对方是否已提出申请
        sql = "select count(*) cnt from friend where uid_1 = " + to + " and uid_2 = " + from + " and status = 1;";
        // cout << sql << endl;
        if (!sptr->query(sql))
        {
            statusCode = DATABASEERROR;
            break;
        }
        try
        {
            if (sptr->next() && "0" != sptr->value(0))
            {
                statusCode = WAITVERITY;
                return 0;
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            statusCode = EXCEPTION;
            break;
        }

        // 检查是否为重复的申请
        sql = "select count(*) from friend where uid_1 = " + from + " and uid_2 = " + to + " and status = 1;";
        // cout << sql << endl;
        if (!sptr->query(sql))
        {
            statusCode = DATABASEERROR;
            break;
        }
        try
        {
            if (sptr->next() && "0" != sptr->value(0))
            {
                statusCode = DUPLICATEFRIENDREQ;
                return 0;
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            statusCode = EXCEPTION;
            break;
        }

        // 添加申请记录
        sql = "insert into friend (uid_1, uid_2, status, lastUpdateTime) values (" + from + ", " + to + ", 1, '" + getDateTime() + "');";
        cout << sql << endl;
        if (!sptr->update(sql))
        {
            statusCode = DATABASEERROR;
            break;
        }
        else 
        {
            statusCode = SUCCESS;
            return 1;
        }
    } while (0);
    
    return -1;
}

std::vector<std::string> IDatabase::getFriendApplicationList(std::string from, int& statusCode, bool& res)
{
    using namespace std;

    ConnectionPool& pool = ConnectionPool::getInstance();
    do
    {
        shared_ptr<MysqlConn> sptr = pool.getConnection();
        if (nullptr == sptr)
        {
            statusCode = DATABASEBUSY;
            break;
        }

        vector<string> data;
        // 查找好友申请（别的用户发起）
        string sql = "select f.id, email, f.status, lastUpdateTime from friend f, user u where uid_2 = " + from + " and uid_1 = u.id and f.status != 0;";
        // cout << sql << endl;
        if (!sptr->query(sql))
        {
            statusCode = DATABASEERROR;
            break;
        }
        try
        {
            while (sptr->next())
                data.emplace_back(sptr->value(0) + "|" + sptr->value(1) + "|" + to_string(stoi(sptr->value(2)) + 2) + "|" + sptr->value(3));
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            statusCode = EXCEPTION;
            break;
        }
        
        // 查找自己发起的申请
        sql = "select f.id, email, f.status, lastUpdateTime from friend f, user u where uid_1 = " + from + " and uid_2 = u.id and f.status != 0;";
        // cout << sql << endl;
        if (!sptr->query(sql))
        {
            statusCode = DATABASEERROR;
            break;
        }
        try
        {
            while (sptr->next())
                data.emplace_back(sptr->value(0) + "|" + sptr->value(1) + "|" + sptr->value(2) + "|" + sptr->value(3));
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            statusCode = EXCEPTION;
            break;
        }
        
        res = true;
        statusCode = SUCCESS;
        return data;
    } while (0);
    
    res = false;
    return {};
}

bool IDatabase::friendVerification(std::string regId, bool flag, int& statusCode)
{
    using namespace std;

    string status = flag ? "0" : "2";
    string sql = "update friend set status =" + status + ", lastUpdateTime = '" + getDateTime() + "' where id = " + regId + ";";
    // cout << sql << endl;
    ConnectionPool& pool = ConnectionPool::getInstance();
    do
    {
        shared_ptr<MysqlConn> sptr = pool.getConnection();
        if (nullptr == sptr)
        {
            statusCode = DATABASEBUSY;
            break;
        }
        
        if (!sptr->update(sql))
        {
            statusCode = DATABASEERROR;
            break;
        }

        statusCode = SUCCESS;
        return true;
    } while (0);
    return false;
}

std::vector<std::string> IDatabase::getFriendList(std::string from, int& statusCode, bool &res)
{
    using namespace std;

    string sql = "select uid_2 id, email from friend f, user u where f.status = 0 and uid_1 = " + from + " and uid_2 = u.id" + 
                 " union " +
                 "select uid_1 id, email from friend f, user u where f.status = 0 and uid_2 = " + from + " and uid_1 = u.id;";
    ConnectionPool& pool = ConnectionPool::getInstance();
    do
    {
        shared_ptr<MysqlConn> sptr = pool.getConnection();
        if (nullptr == sptr) 
        {
            statusCode = DATABASEBUSY;
            break;
        }
        if (!sptr->query(sql))
        {
            statusCode = DATABASEERROR;
            break;
        }

        vector<string> resList;
        try
        {
            while (sptr->next())
                resList.emplace_back(sptr->value(0) + "|" + sptr->value(1));
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            statusCode = EXCEPTION;
            break;
        }
        
        res = true;
        statusCode = SUCCESS;
        return resList;
    } while (0);
    
    res = false;
    return {};
}
