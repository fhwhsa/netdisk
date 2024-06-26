#ifndef MSGTOOLS_H
#define MSGTOOLS_H

#include "msgUnit.h"

#include <QString>
#include <QStringList>

class MsgTools
{
private:
    MsgTools();

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

    /**
     * @brief 生成添加好友请求
     * @param from 申请人邮箱
     * @param to 目标好友邮箱
     * @return
     */
    static MsgUnit *generateAddFriendRequest(QString from, QString to);

    /**
     * @brief 获取munit消息内容中第index行内容，index从0开始
     * @param munit 要获取信息的消息单元
     * @param index 第几行
     * @return index有效则返回对应行内容，否则返回空字符串
     */
    static QString getRow(const MsgUnit* munit, int index);

    /**
     * @brief 获取munit消息内容中所有行的内容
     * @param munit 要获取所有行内容的消息单元
     * @return QStringList对象，存储每一行的内容
     */
    static QStringList getAllRows(const MsgUnit* munit);

};

#endif // MSGTOOLS_H
