#ifndef _IDATABASE_H_
#define _IDATABASE_H_

#include <string>
#include <vector>

/// @brief 数据库交互接口
class IDatabase
{
public:
    /// @brief 服务器下线处理，将所有用户的在线状态改为不在线
    static void serverOffline();
    
    /// @brief 用户身份验证
    /// @param email 登陆邮箱
    /// @param passwd 密码
    /// @param handleInfo 处理信息
    /// @return 成功返回登陆用户的id（>0），否则返回-1
    static int authentication(std::string email, std::string passwd, std::string& handleInfo);

    /// @brief 用户注销
    /// @param id 用户id
    /// @return 成功返回true，否则返回false
    static bool logout(std::string id);

    /// @brief 查找用户
    /// @param key 查找关键字
    /// @param handleInfo 处理信息
    /// @return 成功返回查找用户的id和邮箱，失败返回空字符串
    static std::pair<std::string, std::string> searchUser(std::string key, std::string& handleInfo);

    /// @brief 添加好友申请记录
    /// @param from 申请人id
    /// @param to 目标用户id
    /// @param handleinfo 处理信息
    /// @return 成功返回1,冲突返回0,失败返回-1
    static int addFriendApplication(std::string from, std::string to, std::string& handleinfo);

    /// @brief 获取好友申请记录，包括自己发起和别人发起的
    /// @param from 获取用户的id
    /// @param handleinfo 处理信息
    /// @param res 处理结果
    /// @return res为true时返回为所有记录的信息，否则为空容器
    static std::vector<std::string> getFriendApplicationList(std::string from, std::string& handleinfo, bool res);

private:
    IDatabase();
    IDatabase(const IDatabase& obj);
    IDatabase& operator=(const IDatabase& onj);
};

#endif
