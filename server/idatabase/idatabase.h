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
     * 描述：服务器下线处理，将所有用户的在线状态改为不在线
     */
    static void serverOffline();

    /**
     * 描述：用户身份验证
     * 参数：
     *      email       登陆邮箱
     *      passwd      密码
     *      handleInfo  处理信息
     * 返回值：
     *      成功        登陆用户的id
     *      失败        -1
     */
    static int authentication(std::string email, std::string passwd, std::string& handleInfo);

    /**
     * 描述：用户注销
     * 参数：
     *      id      用户id
     * 返回值：
     *      成功     true
     *      失败     false
     */
    static bool logout(std::string id);

    /**
     * 描述：查找用户
     * 参数：
     *      key         查找关键字
     *      handleInfo  处理信息
     * 返回值：
     *      成功         查找用户的邮箱
     *      失败         空字符串
     */
    static std::string searchUser(std::string key, std::string& handleInfo);

    /**
     * 描述：添加好友申请记录
     * 参数：
     *      from    申请人邮箱
     *      to      目标用户邮箱
     *      handleInfo  处理信息
     * 返回值：
     *      成功            1
     *      重复的添加       0
     *      失败            -1
     */
    static int addFriendApplication(std::string from, std::string to, std::string& handleinfo);

private:
    IDatabase();
    IDatabase(const IDatabase& obj);
    IDatabase& operator=(const IDatabase& onj);
};

#endif