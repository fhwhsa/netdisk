#include "mysqlConn.h"

MysqlConn::MysqlConn()
{
    mysqlconn = mysql_init(nullptr);
    mysql_set_character_set(mysqlconn, "utf8");
}

MysqlConn::~MysqlConn()
{
    if (mysqlres)
    {
        mysql_free_result(mysqlres);
        mysqlres = nullptr;
    }
    mysql_close(mysqlconn);
}

bool MysqlConn::connect(std::string host, std::string user, std::string passwd, std::string dbname, unsigned int port)
{
    mysqlconn = mysql_real_connect(mysqlconn, host.c_str(), user.c_str(), passwd.c_str(), dbname.c_str(), port, nullptr, 0);
    return mysqlconn ? true : false;
}

bool MysqlConn::update(std::string sql)
{
    return 0 == mysql_query(mysqlconn, sql.c_str());
}

bool MysqlConn::query(std::string sql)
{
    initQuery();
    if (mysql_query(mysqlconn, sql.c_str()))
        return false;
    mysqlres = mysql_store_result(mysqlconn);
    if (nullptr == mysqlres)
        return false;
    num = mysql_num_fields(mysqlres);
    mysqlfield = mysql_fetch_field(mysqlres);
    return true;
}

bool MysqlConn::next()
{
    mysqlrow = mysql_fetch_row(mysqlres);
    if (nullptr == mysqlrow)
        return false;
    len = mysql_fetch_lengths(mysqlres);
    return true;
}

bool MysqlConn::hasRes()
{
    return nullptr != mysqlres;
}

unsigned int MysqlConn::getColNum()
{
    return num;
}

std::string MysqlConn::value(unsigned int index)
{
    if (index >= num || nullptr == mysqlrow || index < 0)
        throw std::runtime_error("The over-access or result set is empty!");
    return std::string(mysqlrow[index], len[index]);
}

std::string MysqlConn::value(std::string item)
{
    if (stoindex.empty())
    {
        for (unsigned int i = 0; i < num; ++i)
            stoindex[std::string(mysqlfield[i].name)] = i;
    }
    if (stoindex.end() == stoindex.find(item))
        throw std::runtime_error("No such item!");
    
    return value(stoindex[item]);
}

bool MysqlConn::start_transaction()
{
    return 0 == mysql_query(mysqlconn, "start transaction;");
}

bool MysqlConn::commit()
{
    return 0 == mysql_query(mysqlconn, "commit;");
}

bool MysqlConn::rollback()
{
    return 0 == mysql_query(mysqlconn, "rollback;");
}

void MysqlConn::initQuery()
{
    mysqlfield = nullptr;
    num = 0;
    len = nullptr;
    mysqlrow = nullptr;
    stoindex.clear();
    if (nullptr != mysqlres)
    {
        mysql_free_result(mysqlres);
        mysqlres = nullptr;
    }
}
