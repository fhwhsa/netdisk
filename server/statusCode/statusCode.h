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
#define FILEEXIST               211     /*文件已存在*/

// 数据库错误
#define DATABASEBUSY            301     /*数据库繁忙*/
#define DATABASEERROR           302     /*数据库错误*/

#define EXCEPTION               401     /*异常*/

#define UNKNOWN                 501     /*未知错误*/

#endif