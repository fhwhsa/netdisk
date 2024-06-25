#include "friendpage.h"
#include "ui_friendpage.h"
#include "addfrienddialog.h"
#include "msggenerate.h"

FriendPage::FriendPage(QWidget *parent) :
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
}

void FriendPage::clickTbNewFriend()
{
    AddFriendDialog *afd = new AddFriendDialog(this);
    connect(afd, &AddFriendDialog::_searchUser, [this](QString key){
        emit _sendMsg(MsgGenerate::generateSearchUserRequest(key));
    });
    afd->exec();
    delete afd;
    afd = nullptr;
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

void FriendPage::getMsg(std::shared_ptr<MsgUnit> sptr)
{
    qDebug() << "friendpage getmsg";
    if (MsgType::MSG_TYPE_SEARCHUSER_RESPOND == sptr->msgType)
        emit respondSearch(sptr);
}
