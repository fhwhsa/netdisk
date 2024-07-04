#include "msgtools.h"

#include <string>

MsgTools::MsgTools()
{

}

MsgUnit* MsgTools::msgHandler(const QString &msg, MsgType type)
{
    std::string sstr = msg.toStdString();
    const char* cstr = sstr.c_str();
    size_t num = strlen(sstr.c_str());
    MsgUnit* munit = MsgUnit::make_dataunit(type, num, cstr);
    return munit;
}

MsgUnit *MsgTools::generateLoginRequest(QString email, QString passwd)
{
    QString str = QString("email:%1\r\npasswd:%2\r\n").arg(email).arg(passwd);
    return msgHandler(str, MsgType::MSG_TYPE_LOGIN_REQUEST);
}

MsgUnit *MsgTools::generateLogoutRequest(QString userId)
{
    QString str = QString("id:%1\r\n").arg(userId);
    return msgHandler(str, MsgType::MSG_TYPE_LOGOUT_REQUEST);
}

MsgUnit *MsgTools::generateSearchUserRequest(QString key)
{
    QString str = QString("key:%1\r\n").arg(key);
    return msgHandler(str, MsgType::MSG_TYPE_SEARCHUSER_REQUEST);
}

MsgUnit *MsgTools::generateAddFriendRequest(QString from, QString to)
{
    QString str = QString("from:%1\r\nto:%2\r\n").arg(from).arg(to);
    return msgHandler(str, MsgType::MSG_TYPE_ADDFRIEND_REQUEST);
}

MsgUnit *MsgTools::generateGetFriendApplicationRequest(QString from)
{
    QString str = QString("from:%1\r\n").arg(from);
    return msgHandler(str, MSG_TYPE_GETFRIENDAPPLICATIONLIST_REQUEST);
}

MsgUnit *MsgTools::generateVerifyFriendRequest(QString regId, bool flag)
{
    QString str = QString("%1\r\n%2\r\n").arg(flag ? "accept" : "refuse").arg(regId);
    return msgHandler(str, MSG_TYPE_FRIENDVERIFICATION_REQUEST);
}

MsgUnit *MsgTools::generateGetFriendListRequest(QString from)
{
    QString str = QString("from:%1\r\n").arg(from);
    return msgHandler(str, MsgType::MSG_TYPE_GETFRIENDLIST_REQUEST);
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


