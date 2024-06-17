/**
 * 解析服务器收到的数据单元
 */

#ifndef _MSGPARSING_H_
#define _MSGPARSING_H_

#include "msgUnit.h"

class MsgParsing 
{
private:
    MsgParsing();
    MsgUnit* loginRespond(const MsgUnit* msg) const;

public:
    /**
     * 
     */
    static MsgUnit* parsing(const MsgUnit* msg);
};

#endif