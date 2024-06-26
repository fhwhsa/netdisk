#include "msgtools.h"

#include <string>

MsgTools::MsgTools()
{

}

MsgUnit *MsgTools::generateLoginRequest(QString email, QString passwd)
{
    QString str = QString("email:%1\r\npasswd:%2\r\n").arg(email).arg(passwd);
    std::string sstr = str.toStdString();
    const char* cstr = sstr.c_str();
    size_t num = strlen(cstr);
    MsgUnit* munit = MsgUnit::make_dataunit(MsgType::MSG_TYPE_LOGIN_REQUEST, num, cstr);
    return munit;
}

MsgUnit *MsgTools::generateLogoutRequest(QString userId)
{
    QString str = QString("id:%1\r\n").arg(userId);
    std::string sstr = str.toStdString();
    const char* cstr = sstr.c_str();
    size_t num = strlen(cstr);
    MsgUnit* munit = MsgUnit::make_dataunit(MsgType::MSG_TYPE_LOGOUT_REQUEST, num, cstr);
    return munit;
}

MsgUnit *MsgTools::generateSearchUserRequest(QString key)
{
    QString str = QString("key:%1\r\n").arg(key);
    std::string sstr = str.toStdString();
    const char* cstr = sstr.c_str();
    size_t num = strlen(cstr);
    MsgUnit* munit = MsgUnit::make_dataunit(MsgType::MSG_TYPE_SEARCHUSER_REQUEST, num, cstr);
    return munit;
}

MsgUnit *MsgTools::generateAddFriendRequest(QString from, QString to)
{
    QString str = QString("from:%1\r\nto:%2\r\n").arg(from).arg(to);
    std::string sstr = str.toStdString();
    const char* cstr = sstr.c_str();
    size_t num = strlen(cstr);
    MsgUnit* munit = MsgUnit::make_dataunit(MsgType::MSG_TYPE_ADDFRIEND_REQUEST, num, cstr);
    return munit;
}

QString MsgTools::getRow(const MsgUnit *munit, int index)
{
    QStringList strList = QString((char*)munit->msg).split("\r\n");
    if (index >= strList.size() - 1)
        return "";
    return strList[index];
}

QStringList MsgTools::getAllRows(const MsgUnit *munit)
{
    return QString((char*)munit->msg).split("\r\n");
}


