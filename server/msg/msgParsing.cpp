#include "msgUnit.h"
#include "msgParsing.h"
#include "../wrap/wrap.h"
#include "../idatabase/idatabase.h"

#include <regex>
#include <iostream>
#include <string.h>
// #include <iostream>

std::vector<std::string> MsgParsing::split_string(const std::string &str, std::string reg)
{
    using namespace std;
    vector<string> tokens;
    regex pattern(reg);
    sregex_token_iterator iter(str.begin(), str.end(), pattern, -1);
    sregex_token_iterator end;
    for (; iter != end; iter++)
        tokens.push_back(*iter);
    return tokens; 
}

MsgUnit *MsgParsing::loginRespond(const MsgUnit *munit)
{
    using namespace std;
    string msg((char*)munit->msg);
    vector<string> tokens = split_string(msg, "\r\n");

    if (tokens.size() < 2)
        return nullptr;
    string email = tokens[0].substr(6);
    string passwd = tokens[1].substr(7);

    // cout << email << " " << passwd << "\n" << flush;
    
    MsgUnit* respond = nullptr;
    int id;
    string info;
    string content;
    // 身份验证成功
    if (-1 != (id = IDatabase::authentication(email, passwd, info)))
    {
        // cout << info << endl;
        content = ("success\r\nid:" + to_string(id) + "\r\ninfo:" + info + "\r\n");
    }

    else 
    {
        // cout << info << endl;
        content = "failure\r\ninfo:" + info + "\r\n";
    }

    respond = MsgUnit::make_dataunit(MsgType::MSG_TYPE_LOGIN_RESPOND, strlen(content.c_str()), content.c_str());
    
    return respond;
}

bool MsgParsing::logoutHandler(const MsgUnit *munit)
{
    using namespace std;
    do 
    {
        string str = getRow(munit, 0);
        if (str.size() <= 3)
            break; 
        if (IDatabase::logout(str.substr(3)))
            break;
        return true;
    } while (0);
    return false;
}

MsgUnit *MsgParsing::searchUserRespond(const MsgUnit *munit)
{
    using namespace std;

    string key = getRow(munit, 0);
    if ("" == key || key.length() <= 4)
        return nullptr;
    key = key.substr(4);
    
    string info;
    pair<string, string> id_email = IDatabase::searchUser(key, info);
    string content;
    if ("-1" == id_email.first)
    {
        content = "failure\r\ninfo:" + info + "\r\n";
    }
    else 
    {   
        content = "success\r\nid:" + id_email.first + "\r\nemail:" + id_email.second + "\r\ninfo:\r\n";
    }

    MsgUnit* respond = MsgUnit::make_dataunit(MsgType::MSG_TYPE_SEARCHUSER_RESPOND, strlen(content.c_str()), content.c_str());
    return respond;
}

MsgUnit *MsgParsing::addFriendRespond(const MsgUnit *munit)
{
    using namespace std;

    vector<string> msg = getAllRows(munit);
    if (msg.size() < 2 || msg[0].length() <= 5 || msg[1].length() <= 3)
        return nullptr;

    string from = msg[0].substr(5);
    string to = msg[1].substr(3);
    string info, content;
    int res = IDatabase::addFriendApplication(from, to, info);
    if (1 == res)
    {
        content = "success\r\ninfo:\r\n";
    }
    else if (0 == res)
    {
        content = "conflict\r\ninfo:" + info + "\r\n";
    }
    else
    {
        content = "failure\r\ninfo:" + info + "\r\n";
    }
    MsgUnit* respond = MsgUnit::make_dataunit(MsgType::MSG_TYPE_ADDFRIEND_RESPOND, strlen(content.c_str()), content.c_str());
    return respond;
}

MsgUnit *MsgParsing::getFriendApplicationListRespond(const MsgUnit *munit)
{
    using namespace std;

    string from = getRow(munit, 0);
    if (from.size() <= 5)
        return nullptr;
    from = from.substr(5);
    string info;
    bool res;
    vector<string> list = IDatabase::getFriendApplicationList(from, info, res);
    string content = "";
    if (!res)
    {
        content = "";
        for (const string& str : list)
        {
            content.append(str + "\r\n");
        }
    }
    else 
    {
        content = "failure\r\ninfo:" + info + "\r\n";
    }

    MsgUnit* respond = MsgUnit::make_dataunit(MsgType::MSG_TYPE_GETFRIENDAPPLICATIONLIST_RESPOND, strlen(content.c_str()), content.c_str());
    return respond;
}

MsgUnit *MsgParsing::verifyFriendApplication(const MsgUnit *munit)
{
    using namespace std;

    vector<string> msg = getAllRows(munit);
    if (msg.size() < 3)
        return nullptr;
    
    string op = msg[0];
    string from = msg[1];
    string to = msg[2];
    if ((op != "accept" && op != "refuse") || from.length() <= 5 || to.length() <= 3)
        return nullptr;
    
    from = from.substr(5);
    to = to.substr(3);

    string info, content;
    bool res = IDatabase::friendVerification(from, to, op == "accept" ? 1 : -1, info);
    if (res)
    {
        content = "success\r\ninfo:\r\n";
    }
    else 
    {
        content = "failure\r\ninfo:" + info + "\r\n";
    }
    
    MsgUnit* respond = MsgUnit::make_dataunit(MsgType::MSG_TYPE_FRIENDVERIFICATION_RESPOND, strlen(content.c_str()), content.c_str());
    return respond;
}

MsgUnit *MsgParsing::parsing(const MsgUnit *munit)
{
    // std::cout << (char*)munit->msg << std::endl;
    switch (munit->msgType)
    {
    // 登陆请求
    case MsgType::MSG_TYPE_LOGIN_REQUEST:
        return loginRespond(munit);

    // 退出登陆请求
    case MsgType::MSG_TYPE_LOGOUT_REQUEST:
    {
        logoutHandler(munit);
        return nullptr;
    }

    // 查找用户请求
    case MsgType::MSG_TYPE_SEARCHUSER_REQUEST:
        return searchUserRespond(munit);

    // 添加好友请求
    case MsgType::MSG_TYPE_ADDFRIEND_REQUEST:
        return addFriendRespond(munit);

    // 获取好友申请记录请求
    case MsgType::MSG_TYPE_GETFRIENDAPPLICATIONLIST_REQUEST:
        return getFriendApplicationListRespond(munit);

    // 验证好友请求
    case MsgType::MSG_TYPE_FRIENDVERIFICATION_REQUEST:
        return verifyFriendApplication(munit);

    // 未知请求
    default:
        break;
    }

    return nullptr;
}

std::string MsgParsing::getRow(const MsgUnit *munit, int index)
{
    using namespace std;
    vector<string> res = split_string(std::string((char*)munit->msg), "\r\n");
    if (index < res.size())
        return res[index];
    return "";
}

std::vector<std::string> MsgParsing::getAllRows(const MsgUnit *munit)
{
    return split_string(std::string((char*)munit->msg), "\r\n");
}
