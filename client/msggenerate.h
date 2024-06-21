#ifndef MSGGENERATE_H
#define MSGGENERATE_H

#include "msgUnit.h"

#include <QString>

class MsgGenerate
{
private:
    MsgGenerate();

public:
    /**
     * @brief 生成登陆请求
     * @param 登陆邮箱
     * @param 登陆密码
     * @return 登陆请求数据单元
     */
    static MsgUnit* generateLoginRequest(QString email, QString passwd);

    static MsgUnit* generateLogoutRequest(QString userId);
};

#endif // MSGGENERATE_H
