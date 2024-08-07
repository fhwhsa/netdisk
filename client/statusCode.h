#ifndef _STATUSCODE_H_
#define _STATUSCODE_H_

#define SUCCESS 100 /*成功*/

// 业务结果
#define USERNOTEXIST            201     /*用户不存在*/
#define WRONGPASSWD             202     /*密码错误*/
#define USERISONLINE            203     /*用户处于在线状态*/
#define DUPLICATEFRIENDREQ      204     /*重复的好友请求*/
#define WAITVERITY              205     /*对方已发起好友请求*/
#define FILEORFOLDERNOTEXIST    206     /*文件/文件夹不存在*/
#define ISNOTFOLDER             207     /*非文件夹*/
#define FOLDEREXIST             208     /*文件夹已存在*/
#define FAILURE                 209     /*失败*/
#define FRIENDSHIPS             210     /*好友关系*/

// 数据库错误
#define DATABASEBUSY            301     /*数据库繁忙*/
#define DATABASEERROR           302     /*数据库错误*/

#define EXCEPTION               401     /*异常*/

#include <QString>

static QString getStatusCodeString(QString status)
{
    int s = status.toInt();
    switch (s)
    {
    case SUCCESS:
        return "成功";

    case USERNOTEXIST:
        return "用户不存在";
    case WRONGPASSWD:
        return "密码错误";
    case USERISONLINE:
        return "用户已在线";
    case DUPLICATEFRIENDREQ:
        return "重复的好友请求";
    case WAITVERITY:
        return "对方已发起好友请求，请进行验证";
    case FILEORFOLDERNOTEXIST:
        return "文件或文件夹不存在";
    case ISNOTFOLDER:
        return "非文件夹";
    case FOLDEREXIST:
        return "文件夹已存在";
    case FAILURE:
        return "失败";
    case FRIENDSHIPS:
        return "已添加为好友";

    case DATABASEBUSY:
        return "数据库繁忙，请稍后再试";
    case DATABASEERROR:
        return "数据库错误";

    case EXCEPTION:
        return "服务器异常，请联系开发人员";

    default:
        break;
    }
    return "未知错误";
}

#endif
