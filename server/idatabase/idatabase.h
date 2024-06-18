/**
 * 数据库交互接口
 */

#ifndef _IDATABASE_H_
#define _IDATABASE_H_

#include <string>

class IDatabase
{
public:
    /**
     * 描述：用户身份验证
     * 参数：
     *      email       登陆邮箱
     *      passwd      密码
     * 返回值：
     *      成功        true
     *      失败        false
     */
    static bool authentication(std::string email, std::string passwd);

private:
    IDatabase();
    IDatabase(const IDatabase& obj);
    IDatabase& operator=(const IDatabase& onj);
};

#endif