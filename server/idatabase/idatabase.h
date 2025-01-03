#ifndef _IDATABASE_H_
#define _IDATABASE_H_

#include "../mysql/mysqlConn.h"
#include "../mysql/connectionPool.h"
#include "../statusCode/statusCode.hpp"

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <iomanip>

/// @brief 数据库交互接口
class IDatabase
{
public:

    /// @brief 服务器下线处理，将所有用户的在线状态改为不在线
    static void serverOffline();
    
    /// @brief 用户身份验证
    /// @param email 登陆邮箱
    /// @param passwd 密码
    /// @param statusCode 处理结果状态码
    /// @return 成功返回登陆用户的id（>0），否则返回-1
    static int authentication(std::string email, std::string passwd, int& statusCode);

    /// @brief 用户注销
    /// @param id 用户id
    /// @return 成功返回true，否则返回false
    static bool logout(std::string id);

    /// @brief 查找用户
    /// @param key 查找关键字
    /// @param statusCode 处理结果状态码
    /// @return 成功返回查找用户的id和邮箱，失败返回空字符串
    static std::pair<std::string, std::string> searchUser(std::string key, int& statusCode);

    /// @brief 添加好友申请记录(添加好友)
    /// @param from 用户id
    /// @param to 目标用户id
    /// @param statusCode 处理结果状态码
    /// @return 成功返回1,冲突返回0,失败返回-1
    static int addFriendApplication(std::string from, std::string to, int& statusCode);

    /// @brief 获取好友申请记录，包括自己发起和别人发起的
    /// @param from 用户id
    /// @param statusCode 处理结果状态码
    /// @param res 处理结果
    /// @return res为true时表示执行成功，返回为所有记录的信息；否则为空容器
    static std::vector<std::string> getFriendApplicationList(std::string from, int& statusCode, bool& res);

    /// @brief 同意/拒绝好友申请
    /// @param regId 好友申请记录id
    /// @param flag 同意为true
    /// @param statusCode 处理结果状态码
    /// @return 成功返回true
    static bool friendVerification(std::string regId, bool flag, int& statusCode);

    /// @brief 获取好友列表
    /// @param from 用户id
    /// @param statusCode 处理结果状态码
    /// @param res res为true时表示执行成功，返回为所有记录的信息；否则为空容器
    /// @return 
    static std::vector<std::string> getFriendList(std::string from, int& statusCode, bool& res);

private:
    IDatabase();
    IDatabase(const IDatabase& obj);
    IDatabase& operator=(const IDatabase& onj);

    /// @brief 获取当前时间YYYY-mm-dd HH:MM:SS
    /// @return 
    static std::string getDateTime();
};

#endif
