/**
 * 通信数据单元
*/

#ifndef _MSGUNIT_T_
#define _MSGUNIT_T_

typedef unsigned int uint;

enum MsgType {
    MSG_TYPE_REGIST_REQUEST,    /*注册请求*/
    MSG_TYPE_REGIST_RESPOND,    /*注册响应*/
    MSG_TYPE_LOGIN_REQUEST,     /*登陆请求*/
    MSG_TYPE_LOGIN_RESPOND,     /*登陆响应*/
    MSG_TYPE_LOGOUT_REQUEST     /*退出登陆请求*/
};

constexpr unsigned int msgTypeNum = 5;

struct MsgUnit
{
    uint totalLen;         // 消息单元大小
    MsgType msgType;       // 消息类型
    uint msgLen;           // 消息大小
    int msg[];             // 消息内容

    /**
     * 描述：构造一个MsgUnit
     * 参数：
     *      msgType     消息类型
     *      msgLen      消息大小
     * 返回：
     *      成功          返回MsgUnit指针，指向新创建的对象
     *      失败          空指针
    */
    static MsgUnit* make_dataunit(MsgType msgType, uint msgLen, const char *msg);
};

#endif