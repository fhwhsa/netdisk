#include "friendpage.h"
#include "ui_friendpage.h"
#include "addfrienddialog.h"
#include "msgtools.h"
#include "friendapplicationlist.h"
#include "bubbletips.h"

#include <QEventLoop>
#include <QTimer>
#include <QStringList>

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

void FriendPage::init()
{

}

void FriendPage::iniSignalSlots()
{
    connect(ui->tb_newFriend, &QToolButton::clicked, this, &FriendPage::clickTbNewFriend);
    connect(ui->tb_search, &QToolButton::clicked, this, &FriendPage::clickTbSearch);
    connect(ui->tb_clear, &QToolButton::clicked, this, &FriendPage::clickTbClear);
    connect(ui->tb_send, &QToolButton::clicked, this, &FriendPage::clickTbSend);
    connect(ui->tb_notification, &QToolButton::clicked, this, &FriendPage::clikcTbNotification);
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

void FriendPage::clikcTbNotification()
{
    FriendApplicationList *fal = new FriendApplicationList(this);
    QMetaObject::Connection conn = connect(fal, &FriendApplicationList::getApplicaionList, [this](){
        emit _sendMsg(MsgTools::generateGetFriendApplicationRequest(userId));
    });
    fal->refreshManually();
    fal->exec();
    disconnect(conn);
    if (nullptr != fal)
    {
        delete fal;
        fal = nullptr;
    }
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

}
