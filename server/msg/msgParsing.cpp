#include "msgUnit.h"
#include "msgParsing.h"
#include "../wrap/wrap.h"
#include "../idatabase/idatabase.h"

#include <regex>
#include <iostream>
#include <string.h>
#include <iostream>

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

    string email = tokens[0].substr(6);
    string passwd = tokens[1].substr(7);

    // cout << email << " " << passwd << "\n" << flush;
    
    MsgUnit* respond = nullptr;
    // 身份验证成功
    if (IDatabase::authentication(email, passwd))
    {
        // cout << "yes" << endl;
        const char* content = "success\r\n";
        respond = MsgUnit::make_dataunit(MsgType::MSG_TYPE_LOGIN_RESPOND, strlen(content), content);
    }

    else 
    {
        // cout << "no" << endl;
        const char* content = "failure\r\n";
        respond = MsgUnit::make_dataunit(MsgType::MSG_TYPE_LOGIN_RESPOND, strlen(content), content);
    }

    return respond;
}

MsgUnit *MsgParsing::parsing(const MsgUnit *munit)
{
    switch (munit->msgType)
    {
    // 登陆请求
    case MsgType::MSG_TYPE_LOGIN_REQUEST:
        return loginRespond(munit);
    

    default:
        break;
    }

    return nullptr;
}
