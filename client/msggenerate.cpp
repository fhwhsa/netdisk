#include "msggenerate.h"

#include <string>

MsgGenerate::MsgGenerate()
{

}

MsgUnit *MsgGenerate::generateLoginRequest(QString email, QString passwd)
{
    QString str = QString("email:%1\r\npasswd:%2\r\n").arg(email).arg(passwd);
    std::string sstr = str.toStdString();
    const char* cstr = sstr.c_str();
    size_t num = strlen(cstr);
    MsgUnit* munit = MsgUnit::make_dataunit(MsgType::MSG_TYPE_LOGIN_REQUEST, num, cstr);
    return munit;
}

MsgUnit *MsgGenerate::generateLogoutRequest(QString userId)
{
    QString str = QString("id:%1\r\n").arg(userId);
    std::string sstr = str.toStdString();
    const char* cstr = sstr.c_str();
    size_t num = strlen(cstr);
    MsgUnit* munit = MsgUnit::make_dataunit(MsgType::MSG_TYPE_LOGOUT_REQUEST, num, cstr);
    return munit;
}
