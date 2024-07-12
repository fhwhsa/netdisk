#ifndef MSGTOOLS_H
#define MSGTOOLS_H

#include "msgUnit.h"

#include <QString>
#include <QStringList>

class MsgTools
{
private:
    MsgTools();

    static MsgUnit* msgHandler(const QString& msg, MsgType type);

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
     * @param from 申请人id
     * @param to 目标好友id
     * @return
     */
    static MsgUnit* generateAddFriendRequest(QString from, QString to);

    /**
     * @brief 生成获取好友申请记录请求
     * @param from 申请人id
     * @return
     */
    static MsgUnit* generateGetFriendApplicationRequest(QString from);

    /**
     * @brief 生成验证好友申请请求
     * @param regId 好友申请记录id
     * @param flag 同意/拒绝
     * @return
     */
    static MsgUnit* generateVerifyFriendRequest(QString regId, bool flag);

    /**
     * @brief 生成获取好友列表请求
     * @param from 申请人id
     * @return
     */
    static MsgUnit* generateGetFriendListRequest(QString from);

    /**
     * @brief 生成获取文件夹内容请求
     * @param path 文件夹路径
     * @return
     */
    static MsgUnit* generateGetFolderContentRequest(QString path);

    /**
     * @brief 生成创建文件夹请求
     * @param path 目标路径
     * @param name 新文件夹名
     * @return
     */
    static MsgUnit* generateCreateFolderRequest(QString path, QString name);

    /**
     * @brief 生成重命名文件/文件夹请求
     * @param path 目标文件/文件夹路径
     * @param newName 新名称
     * @return
     */
    static MsgUnit* generateRenameFileOrFolderRequest(QString path, QString newName);

    /**
     * @brief 生成删除文件/文件夹请求
     * @param path 目标文件/文件夹路径
     * @return
     */
    static MsgUnit* generateDeleteFileOrFolderRequest(QString path);

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
