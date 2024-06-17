/**
 * 数据库交互接口
 */

#ifndef _IDATABASE_H_
#define _IDATABASE_H_

class IDatabase
{
private:
    IDatabase();
    static IDatabase instance; 

public:
    static IDatabase& getInstance();
};

#endif