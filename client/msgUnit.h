#ifndef _MSGUNIT_T_
#define _MSGUNIT_T_

typedef unsigned int uint;

enum MsgType {
    /// @brief 注册请求
    MSG_TYPE_REGIST_REQUEST,
    /// @brief 注册响应
    MSG_TYPE_REGIST_RESPOND,
    /// @brief 登陆请求
    MSG_TYPE_LOGIN_REQUEST,
    /// @brief 登陆响应
    MSG_TYPE_LOGIN_RESPOND,
    /// @brief 退出登陆请求
    MSG_TYPE_LOGOUT_REQUEST,
    /// @brief 查找用户请求
    MSG_TYPE_SEARCHUSER_REQUEST,
    /// @brief 查找用户响应
    MSG_TYPE_SEARCHUSER_RESPOND,
    /// @brief 添加好友请求
    MSG_TYPE_ADDFRIEND_REQUEST,
    /// @brief 添加好友响应
    MSG_TYPE_ADDFRIEND_RESPOND,
    /// @brief 获取好友申请记录请求
    MSG_TYPE_GETFRIENDAPPLICATIONLIST_REQUEST,
    /// @brief 获取好友申请记录响应
    MSG_TYPE_GETFRIENDAPPLICATIONLIST_RESPOND,
    /// @brief 好友验证（同意/拒绝）请求
    MSG_TYPE_FRIENDVERIFICATION_REQUEST,
    /// @brief 好友验证响应
    MSG_TYPE_FRIENDVERIFICATION_RESPOND
};

constexpr unsigned int msgTypeNum = 13;

/// @brief 通信数据单元
struct MsgUnit
{
    /// @brief 消息单元大小
    uint totalLen;
    /// @brief 消息类型
    MsgType msgType;
    /// @brief 消息大小
    uint msgLen;
    /// @brief 消息内容
    int msg[];

    /// @brief 构造一个MsgUnit
    /// @param msgType 消息类型
    /// @param msgLen 消息大小
    /// @param msg 消息内容
    /// @return 成功返回MsgUnit指针，指向新创建的对象，失败返回空指针
    static MsgUnit* make_dataunit(MsgType msgType, uint msgLen, const char *msg);
};

#endif
