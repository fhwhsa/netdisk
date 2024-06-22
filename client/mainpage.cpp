#include "mainpage.h"
#include "ui_mainpage.h"
#include "msgUnit.h"
#include "msggenerate.h"

#include <QDebug>
#include <QPair>
#include <QIcon>
#include <QStackedWidget>
#include <QMessageBox>

MainPage::MainPage(QString _userId, QString _userEmail, QTcpSocket* _socket, QWidget *parent) :
    userId(_userId),
    userEmail(_userEmail),
    socket(_socket),
    QWidget(parent),
    ui(new Ui::MainPage)
{
    ui->setupUi(this);
//    qDebug() << userId;
    init();
    iniSignalSlots();
}

MainPage::~MainPage()
{
    qDebug() << "mainpage delete";
    delete ui;

    delete folderPage;
    delete transmitPage;
    delete friendPage;
}

void MainPage::init()
{
    this->setMinimumSize(1100, 700);

    // 设置按钮背景为透明
    QString style = "QToolButton { background-color: transparent; border: none; }";
    ui->tb_folder->setStyleSheet(style);
    ui->tb_transmit->setStyleSheet(style);
    ui->tb_friend->setStyleSheet(style);
    ui->tb_logout->setStyleSheet(style);
    ui->tb_setting->setStyleSheet(style);
    ui->tb_userInfo->setStyleSheet(style);

    // 左菜单三个工具按钮的（选中/未选中）图标
    btnIcon.insert(ui->tb_folder, QPair(QIcon(":/img/res/img/folder_selected.png"), QIcon(":/img/res/img/folder_unselected.png")));
    btnIcon.insert(ui->tb_transmit, QPair(QIcon(":/img/res/img/transmit_selected.png"), QIcon(":/img/res/img/transmit_unselected.png")));
    btnIcon.insert(ui->tb_friend, QPair(QIcon(":/img/res/img/friend_selected.png"), QIcon(":/img/res/img/friend_unselected.png")));

    // 创建功能页对象
    folderPage = new FolderPage();
    transmitPage = new TransmitPage();
    friendPage = new FriendPage();
    ui->funcPanel->addWidget(folderPage);
    ui->funcPanel->setCurrentWidget(folderPage);
    ui->funcPanel->addWidget(transmitPage);
    ui->funcPanel->addWidget(friendPage);

    // 初始页面为文件页
    currSelectedBtn = ui->tb_folder;
    ui->tb_folder->setIcon(btnIcon.value(currSelectedBtn).first);
    ui->funcPanel->setCurrentWidget(folderPage);
}

void MainPage::iniSignalSlots()
{
    connect(ui->tb_folder, &QToolButton::clicked, this, &MainPage::clickTbfolder);
    connect(ui->tb_transmit, &QToolButton::clicked, this, &MainPage::clickTbtransmit);
    connect(ui->tb_friend, &QToolButton::clicked, this, &MainPage::clickTbfriend);
    connect(ui->tb_logout, &QToolButton::clicked, this, &MainPage::clickTblogout);
    connect(ui->tb_setting, &QToolButton::clicked, this, &MainPage::clickTbsetting);
    connect(ui->tb_userInfo, &QToolButton::clicked, this, &MainPage::clickTbUserInfo);
}

void MainPage::clickTbfolder()
{
    if (currSelectedBtn == ui->tb_folder)
        return;

    // 设置按钮样式
    currSelectedBtn->setIcon(btnIcon.value(currSelectedBtn).second);
    currSelectedBtn = ui->tb_folder;
    currSelectedBtn->setIcon(btnIcon.value(currSelectedBtn).first);
    ui->funcPanel->setCurrentWidget(folderPage);
}

void MainPage::clickTbtransmit()
{
    if (currSelectedBtn == ui->tb_transmit)
        return;

    // 设置按钮样式
    currSelectedBtn->setIcon(btnIcon.value(currSelectedBtn).second);
    currSelectedBtn = ui->tb_transmit;
    currSelectedBtn->setIcon(btnIcon.value(currSelectedBtn).first);
    ui->funcPanel->setCurrentWidget(transmitPage);
}

void MainPage::clickTbfriend()
{
    if (currSelectedBtn == ui->tb_friend)
        return;

    // 设置按钮样式
    currSelectedBtn->setIcon(btnIcon.value(currSelectedBtn).second);
    currSelectedBtn = ui->tb_friend;
    currSelectedBtn->setIcon(btnIcon.value(currSelectedBtn).first);
    ui->funcPanel->setCurrentWidget(friendPage);
}

void MainPage::clickTblogout()
{
    if (QMessageBox::No == QMessageBox::question(this, " ", "退出登陆？", QMessageBox::Yes | QMessageBox::No))
        return;
    MsgUnit* munit = MsgGenerate::generateLogoutRequest(this->userId);
//    qDebug() << (char*)munit->msg;
    socket->write((char*)munit, munit->totalLen);
    emit mainPageClosed();
}

void MainPage::clickTbsetting()
{

}

void MainPage::clickTbUserInfo()
{
    QMessageBox::information(this, "账号信息", QString("id:%1\nemail:%2").arg(userId).arg(userEmail));
}
