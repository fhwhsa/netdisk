#include "addfrienddialog.h"
#include "ui_addfrienddialog.h"
#include "bubbletips.h"
#include "friendpage.h"
#include "msgtools.h"
#include "respondwatcher.h"
#include "statusCode.h"

#include <QMessageBox>
#include <QDebug>
#include <QPoint>
#include <memory>

AddFriendDialog::AddFriendDialog(QString _userId, QString _userEmail, FriendPage *_parent) :
    userId(_userId),
    userEmail(_userEmail),
    parent(_parent),
    ui(new Ui::AddFriendDialog)
{
    ui->setupUi(this);

    init();
    iniSignalSlots();
}

AddFriendDialog::~AddFriendDialog()
{
    delete ui;
}

void AddFriendDialog::init()
{
    this->setWindowTitle(QString("新好友"));
    QString style = "background-color: transparent; border: none;";
    ui->targetKey->setStyleSheet(style);
    ui->tb_search->setStyleSheet(style);
    ui->tb_add->setStyleSheet("background-color: transparent; border: none;");
    ui->inputBox->setStyleSheet("QGroupBox {"
                                "border: 2px solid black;"
                                "border-radius: 10px;"
                                "margin-top: 20px;"
                                "}");
    ui->resultBox->setStyleSheet("QGroupBox {"
                                 "border-radius: 10px;"
                                 "margin-top: 20px;"
                                 "}");
    ui->resultBox->hide();
    ui->errorBox->hide();

    QPoint pPos = parent->pos();
    this->move(pPos.rx() + parent->width() / 2 - this->width() / 2, pPos.ry() + parent->height() / 2 - this->height() / 2);

    centerPos = this->pos() + QPoint(this->width() / 2, this->height() / 2);
}

void AddFriendDialog::iniSignalSlots()
{
    connect(ui->tb_search, &QToolButton::clicked, this, &AddFriendDialog::clickTbSearch);
    connect(ui->tb_add, &QToolButton::clicked, this, &AddFriendDialog::clickTbAdd);
}

void AddFriendDialog::clickTbSearch()
{
    QString text = ui->targetKey->text();
    if (text.isEmpty())
    {
        BubbleTips::showBubbleTips("请输入搜索内容", 2, this);
        return;
    }

    RespondWatcher::create(parent, SIGNAL(respondSearch(std::shared_ptr<MsgUnit>)), "等待响应超时，请检查网络", 3, centerPos,
                           [this](std::shared_ptr<MsgUnit> sptr){
                               getSearchUserRespond(sptr);
                           });

    emit _searchUser(text);
}

void AddFriendDialog::clickTbAdd()
{
    QString target = ui->resultEmail->text();
    target = target.left(target.indexOf(':'));
    if (target == userId)
    {
        BubbleTips::showBubbleTips("不可以添加自己为好友", 2, this);
        return;
    }

    RespondWatcher::create(parent, SIGNAL(respondAddFriend(std::shared_ptr<MsgUnit>)), "等待响应超时，请检查网络", 3, centerPos,
                           [this](std::shared_ptr<MsgUnit> sptr){
                                getAddFriendRespond(sptr);
                           });

    emit _addFriend(target);
}

void AddFriendDialog::getSearchUserRespond(std::shared_ptr<MsgUnit> sptr)
{
    ui->errorBox->hide();
    ui->resultBox->hide();
    QString msg = (char*)sptr->msg;
    qDebug() << msg;
    if (msg.first(7) == "success")
    {
        QString id = MsgTools::getRow(sptr.get(), 1);
        QString email = MsgTools::getRow(sptr.get(), 2);
        if ("" == id || "" == email)
        {
            ui->errInfo->setText("数据传输损坏");
            ui->errorBox->show();
        }
        else
        {
            ui->resultEmail->setText(QString("%1:%2").arg(id.mid(3)).arg(email.mid(6)));
            ui->resultBox->show();
        }
    }
    else
    {
        QString info = MsgTools::getRow(sptr.get(), 1);
        if ("" == info)
            info = "数据传输损坏";
        QString statusCode = MsgTools::getRow(sptr.get(), 1);
        if (statusCode.length() <= 7)
            info = "通信错误";
        else
            info = getStatusCodeString(statusCode.mid(7));
        return;

        ui->errInfo->setText(info);
        ui->errorBox->show();
    }
}

void AddFriendDialog::getAddFriendRespond(std::shared_ptr<MsgUnit> sptr)
{
    QStringList list = MsgTools::getAllRows(sptr.get());
    if (list.size() < 3)
    {
        BubbleTips::showBubbleTips("数据传输错误", 2, this);
        return;
    }

    QString statusCode = list[1];
    if ("success" == list[0])
    {
        BubbleTips::showBubbleTips("添加成功，等待对方通过申请", 3, this);
        return;
    }

    if (statusCode.length() <= 7)
    {
        BubbleTips::showBubbleTips("数据传输错误", 2, this);
        return;
    }
    if ("conflict" == list[0])
    {
        BubbleTips::showBubbleTips(getStatusCodeString(statusCode.mid(7)), 3, this);
    }
    else if ("failure" == list[0])
    {
        BubbleTips::showBubbleTips(getStatusCodeString(statusCode.mid(7)), 3, this);
    }
    else ;
}
