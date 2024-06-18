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

public:
    /**
     * 描述：解析消息体，并返回对应的响应
     * 返回值：对应的响应消息体
     */
    static MsgUnit* parsing(const MsgUnit* munit);

    
};

#endif