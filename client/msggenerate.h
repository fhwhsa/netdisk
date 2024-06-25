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
     * @param email 登陆邮箱
     * @param passwd 登陆密码
     * @return 登陆请求数据单元
     */
    static MsgUnit* generateLoginRequest(QString email, QString passwd);

    /**
     * @brief 生成退出登陆
     * @param userId 用户id
     * @return
     */
    static MsgUnit* generateLogoutRequest(QString userId);

    /**
     * @brief 生成查询用户请求
     * @param key 查询关键字，可以为用户id或邮箱
     * @return
     */
    static MsgUnit* generateSearchUserRequest(QString key);
};

#endif // MSGGENERATE_H
