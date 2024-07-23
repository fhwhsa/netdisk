#include "friendpage.h"
#include "ui_friendpage.h"
#include "addfrienddialog.h"
#include "msgtools.h"
#include "friendapplicationlist.h"
#include "bubbletips.h"
#include "flistitemwidget.h"
#include "alistitemwidget.h"
#include "respondwatcher.h"
#include "statusCode.h"

#include <QEventLoop>
#include <QTimer>
#include <QStringList>
#include <QtConcurrent/QtConcurrent>
#include <functional>
#include <QListWidgetItem>
#include <QSize>
#include <QPoint>

FriendPage::FriendPage(QString _userId, QString _userEmail, QWidget *parent) :
    userId(_userId),
    userEmail(_userEmail),
    QWidget(parent),
    ui(new Ui::FriendPage)
{
    ui->setupUi(this);

    ui->chatBox->hide();

    init();
    iniSignalSlots();
}

FriendPage::~FriendPage()
{
    delete ui;
}

void FriendPage::refreshFriendListManually()
{
    clickTbFlushFriendList();
}

void FriendPage::init()
{

}

void FriendPage::iniSignalSlots()
{
    connect(ui->tb_newFriend, &QToolButton::clicked, this, &FriendPage::clickTbNewFriend);
    connect(ui->tb_search, &QToolButton::clicked, this, &FriendPage::clickTbSearch);
    connect(ui->tb_clear, &QToolButton::clicked, this, &FriendPage::clickTbClear);
    connect(ui->tb_send, &QToolButton::clicked, this, &FriendPage::clickTbSend);
    connect(ui->tb_notification, &QToolButton::clicked, this, &FriendPage::clickTbNotification);
    connect(ui->tb_flushFriendList, &QToolButton::clicked, this, &FriendPage::clickTbFlushFriendList);
}

void FriendPage::flushFriendList(std::shared_ptr<MsgUnit> sptr)
{
    QStringList list = MsgTools::getAllRows(sptr.get());

    if (list.size() >= 2 && list[0] == "failure")
    {
        QString statusCode = list[1];
        if (statusCode.length() <= 7)
            BubbleTips::showBubbleTips("通信错误", 2, this);
        else
            BubbleTips::showBubbleTips(getStatusCodeString(statusCode.mid(7)), 2, this);
        return;
    }

    for (int i = ui->friendList->count() - 1; ~i; --i)
    {
        QListWidgetItem* item = ui->friendList->item(i);
        FListItemWidget* iw = (FListItemWidget*)ui->friendList->itemWidget(item);
        if (nullptr != iw)
        {
            iw->deleteLater();
            ui->friendList->setItemWidget(item, nullptr);
        }
        ui->friendList->takeItem(i);
    }

    for (const QString& str : list)
    {
        if (1 != str.count('|'))
            continue;
        QStringList para = str.split('|');
        QListWidgetItem* item = new QListWidgetItem(ui->friendList);
        FListItemWidget* iw = new FListItemWidget(para[1], "", para[0], para[1], ui->friendList);
        ui->friendList->setItemWidget(item, iw);
        item->setSizeHint(QSize(ui->friendList->width() - 10, iw->height()));
    }
}

void FriendPage::clickTbNewFriend()
{
    AddFriendDialog *afd = new AddFriendDialog(userId, userEmail, this);
    QMetaObject::Connection conn1 = connect(afd, &AddFriendDialog::_searchUser, [this](QString key){
        emit _sendMsg(MsgTools::generateSearchUserRequest(key));
    });
    QMetaObject::Connection conn2 = connect(afd, &AddFriendDialog::_addFriend, [this](QString to){
        emit _sendMsg(MsgTools::generateAddFriendRequest(userId, to));
    });
    afd->exec();
    disconnect(conn1);
    disconnect(conn2);
    if (nullptr != afd)
    {
        delete afd;
        afd = nullptr;
    }
}

void FriendPage::clickTbSearch()
{

}

void FriendPage::clickTbClear()
{
    ui->inputBox->clear();
}

void FriendPage::clickTbSend()
{

}

void FriendPage::clickTbNotification()
{
    FriendApplicationList *fal = new FriendApplicationList(this);
    QMetaObject::Connection conn1 = connect(fal, &FriendApplicationList::getApplicaionList, [this](){
        emit _sendMsg(MsgTools::generateGetFriendApplicationRequest(userId));
    });
    QMetaObject::Connection conn2 = connect(fal, &FriendApplicationList::verifyFriend, [this](QString regId, bool flag){
        emit _sendMsg(MsgTools::generateVerifyFriendRequest(regId, flag));
    });
    fal->refreshManually();
    fal->exec();
    disconnect(conn1);
    if (nullptr != fal)
    {
        delete fal;
        fal = nullptr;
    }
}

void FriendPage::clickTbFlushFriendList()
{
    RespondWatcher::create(this, SIGNAL(respondGetFriendList(std::shared_ptr<MsgUnit>)), "好友列表刷新超时", 3,
            QPoint(this->pos().rx() + this->width() / 2, this->pos().ry() + this->height() / 2),
        [this](std::shared_ptr<MsgUnit> sptr){
        flushFriendList(sptr);
    });
    emit _sendMsg(MsgTools::generateGetFriendListRequest(userId));
}

void FriendPage::getMsg(std::shared_ptr<MsgUnit> sptr)
{
    qDebug() << "friendpage getmsg";
    if (MsgType::MSG_TYPE_SEARCHUSER_RESPOND == sptr->msgType)
        emit respondSearch(sptr);
    else if (MsgType::MSG_TYPE_ADDFRIEND_RESPOND == sptr->msgType)
        emit respondAddFriend(sptr);
    else if (MsgType::MSG_TYPE_GETFRIENDAPPLICATIONLIST_RESPOND == sptr->msgType)
        emit respondGetFriendApplication(sptr);
    else if (MsgType::MSG_TYPE_FRIENDVERIFICATION_RESPOND == sptr->msgType)
        emit respondVerify(sptr);
    else if (MsgType::MSG_TYPE_GETFRIENDLIST_RESPOND == sptr->msgType)
        emit respondGetFriendList(sptr);
}
