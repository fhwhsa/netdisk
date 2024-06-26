/**
 * 解析服务器收到的数据单元
 */

#ifndef _MSGPARSING_H_
#define _MSGPARSING_H_

#include "msgUnit.h"

#include <vector>
#include <string>

class MsgParsing 
{
private:
    MsgParsing();

    /**
     * 描述：以reg分割字符串str
     * 返回值：返回一个vector，包含被分割符分割后的子字符串
     */
    static std::vector<std::string> split_string(const std::string& str, std::string reg);

    /**
     * 描述：处理登陆请求
     * 返回值：返回登陆响应
     */
    static MsgUnit* loginRespond(const MsgUnit* munit);

    /**
     * 描述：处理退出登陆请求
     */
    static bool logoutHandler(const MsgUnit* munit);

    /**
     * 描述：查找用户
     * 返回值：返回查找用户响应
     */
    static MsgUnit* searchUserRespond(const MsgUnit* munit);

    /**
     * 描述：添加好友
     * 返回值：返回添加好友响应
     */
    static MsgUnit* addFriendRespond(const MsgUnit* munit);

public:
    /**
     * 描述：解析消息体，并返回对应的响应
     * 返回值：对应的响应消息体
     */
    static MsgUnit* parsing(const MsgUnit* munit);

    /**
     * 描述：获取munit消息内容中第index行内容，index从0开始
     * 建议：当仅需要获取其中一行时建议调用该方法，否则调用allValue方法
     * 参数：
     *      munit：要获取信息的消息单元
     *      index：第几行
     * 返回值：
     *      index有效则返回对应行内容，否则返回空字符串
     */
    static std::string getRow(const MsgUnit* munit, int index);
    
    /**
     * 描述：获取munit消息内容中所有行的内容
     * 参数：
     *      munit：要获取所有行内容的消息单元
     * 返回值：
     *      vector容器，存储每一行的内容
     */
    static std::vector<std::string> getAllRows(const MsgUnit* munit);
};

#endif