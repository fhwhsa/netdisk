/**
 * 解析服务器收到的数据单元
 */

#ifndef _MSGPARSING_H_
#define _MSGPARSING_H_

#include "../connResources/connResources.h"
#include "msgUnit.h"

#include <vector>
#include <string>

class MsgParsing 
{
private:
    MsgParsing();

    /// @brief 以reg分割字符串str
    /// @param str 要分割的字符串
    /// @param reg 分隔字符串
    /// @return 返回一个vector，包含被分割符分割后的子字符串
    static std::vector<std::string> split_string(const std::string& str, std::string reg);

    /// @brief 处理登陆请求
    /// @param munit 通信消息
    /// @return 返回登陆响应
    static MsgUnit* loginRespond(const MsgUnit* munit);

    /// @brief 处理退出登陆请求
    /// @param munit 通信消息
    /// @return 成功返回true
    static bool logoutHandler(const MsgUnit* munit);

    /// @brief 查找用户
    /// @param munit 通信消息
    /// @return 返回查找用户响应消息
    static MsgUnit* searchUserRespond(const MsgUnit* munit);

    /// @brief 添加好友
    /// @param munit 通信消息
    /// @return 返回添加好友响应消息
    static MsgUnit* addFriendRespond(const MsgUnit* munit);

    /// @brief 处理获取好友申请记录请求
    /// @param munit 通信消息
    /// @return 返回好友申请记录
    static MsgUnit* getFriendApplicationListRespond(const MsgUnit* munit);

    /// @brief 验证好友请求
    /// @param munit 通信消息
    /// @return 返回处理响应
    static MsgUnit* verifyFriendApplication(const MsgUnit* munit);

    /// @brief 处理获取好友列表请求
    /// @param munit 通信消息
    /// @return 返回对应用户的好友列表
    static MsgUnit* getFriendListRespond(const MsgUnit* munit);

    /// @brief 处理获取文件夹内容请求
    /// @param munit 通信消息
    /// @return 返回处理响应
    static MsgUnit* getFolderContentRespond(const MsgUnit* munit);

    /// @brief 处理创建文件夹请求
    /// @param munit 通信消息
    /// @return 返回处理响应
    static MsgUnit* createFolderRespond(const MsgUnit* munit);

    /// @brief 处理重命名文件/文件夹请求
    /// @param munit 通信消息
    /// @return 返回处理响应
    static MsgUnit* renameFileFolderRespond(const MsgUnit* munit);

    /// @brief 处理删除文件/文件夹请求
    /// @param munit 通信消息
    /// @return 返回处理响应
    static MsgUnit* deleteFileFolderRespond(const MsgUnit* munit);

    /// @brief 处理上传文件请求
    /// @param munit 通信信息
    /// @return 返回处理响应
    static MsgUnit* uploadFileRespond(const MsgUnit* munit, ConnResources& ur);

public:

    /// @brief 解析通信消息，并返回对应的响应
    /// @param munit 通信消息
    /// @param ur 用户系统资源
    /// @return 对应的响应消息
    static MsgUnit* parsing(const MsgUnit* munit, ConnResources& ur);

    /// @brief 获取munit消息内容中第index行内容，index从0开始。建议当仅需要获取其中一行时建议调用该方法，否则调用allValue方法
    /// @param munit 要获取信息的消息单元
    /// @param index 第几行
    /// @return index有效则返回对应行内容，否则返回空字符串
    static std::string getRow(const MsgUnit* munit, int index);

    /// @brief 获取munit消息内容中所有行的内容
    /// @param munit 要获取所有行内容的消息单元
    /// @return vector容器，存储每一行的内容
    static std::vector<std::string> getAllRows(const MsgUnit* munit);
};

#endif