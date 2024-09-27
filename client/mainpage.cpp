#include "mainpage.h"
#include "ui_mainpage.h"
#include "msgUnit.h"
#include "msgtools.h"
#include "bubbletips.h"

#include <QDebug>
#include <QPair>
#include <QIcon>
#include <QStackedWidget>
#include <QMessageBox>
#include <string.h>
#include <QDateTime>
#include <QPoint>
#include <QtConcurrent/QtConcurrent>
#include <functional>

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

    folderPage->refreshFileListManually();
}

MainPage::~MainPage()
{
    delete folderPage;
    delete transmitPage;
    delete friendPage;

    delete ui;
}

void MainPage::init()
{
    this->setMinimumSize(1100, 700);
    this->setWindowTitle(" ");

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
    folderPage = new FolderPage(userId, userEmail, this);
    transmitPage = new TransmitPage(this);
    friendPage = new FriendPage(userId, userEmail, this);
    ui->funcPanel->addWidget(folderPage);
    ui->funcPanel->setCurrentWidget(folderPage);
    ui->funcPanel->addWidget(transmitPage);
    ui->funcPanel->addWidget(friendPage);

    // 初始页面为文件页
    currSelectedBtn = ui->tb_folder;
    ui->tb_folder->setIcon(btnIcon.value(currSelectedBtn).first);
    ui->funcPanel->setCurrentWidget(folderPage);

    revMunit = nullptr;
}

void MainPage::iniSignalSlots()
{
    connect(ui->tb_folder, &QToolButton::clicked, this, &MainPage::clickTbfolder);
    connect(ui->tb_transmit, &QToolButton::clicked, this, &MainPage::clickTbtransmit);
    connect(ui->tb_friend, &QToolButton::clicked, this, &MainPage::clickTbfriend);
    connect(ui->tb_logout, &QToolButton::clicked, this, &MainPage::clickTblogout);
    connect(ui->tb_setting, &QToolButton::clicked, this, &MainPage::clickTbsetting);
    connect(ui->tb_userInfo, &QToolButton::clicked, this, &MainPage::clickTbUserInfo);

    connect(folderPage, &FolderPage::_sendMsg, this, &MainPage::sendMsg);
    connect(this, &MainPage::newMunit, folderPage, &FolderPage::getMsg);
    connect(folderPage, &FolderPage::deliverUploadTask, [this](QString filepath, QString diskpath){
        transmitPage->addUploadTask(filepath, diskpath);
    });
    connect(folderPage, &FolderPage::deliverDownloadTask, [this](QString filepath, qint64 filesize){
        transmitPage->addDownloadTask(filepath, filesize);
    });

    connect(transmitPage, &TransmitPage::_sendMsg, this, &MainPage::sendMsg);
    connect(this, &MainPage::newMunit, transmitPage, &TransmitPage::getMsg);

    connect(friendPage, &FriendPage::_sendMsg, this, &MainPage::sendMsg);
    connect(this, &MainPage::newMunit, friendPage, &FriendPage::getMsg);

    connect(socket, &QTcpSocket::readyRead, this, &MainPage::recvMsg);
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

    // 刷新文件列表
    folderPage->refreshFileListManually();
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

    // 刷新好友列表
    friendPage->refreshFriendListManually();
}

void MainPage::clickTblogout()
{
    if (QMessageBox::No == QMessageBox::question(this, " ", "退出登陆？", QMessageBox::Yes | QMessageBox::No))
        return;
    MsgUnit* munit = MsgTools::generateLogoutRequest(this->userId);
    socket->write((char*)munit, munit->totalLen);
    emit mainPageClosed();
}

void MainPage::clickTbsetting()
{
    SettingDialog* settingDialog = new SettingDialog(this);
    settingDialog->show();
}

void MainPage::clickTbUserInfo()
{
    QMessageBox::information(this, "账号信息", QString("id:%1\nemail:%2").arg(userId).arg(userEmail));
}

void MainPage::sendMsg(MsgUnit *munit)
{
//    qDebug() << (char*)munit->msg;
//    qDebug() << munit->msgType;

    if (-1 == socket->write((char*)munit, munit->totalLen))
        BubbleTips::showBubbleTips(socket->errorString(), 2, this);
    try {
        free(munit);
    } catch (const std::exception& e) {
        qDebug() << e.what();
    }
}

void MainPage::recvMsg()
{
//    QString now = QDateTime::currentDateTime().toString();
//    qDebug() << "main get" << now;
    if (nullptr == revMunit && socket->bytesAvailable() >= 4)
    {
        QByteArray ba = socket->read(4);
        uint size;
        memcpy(&size, ba.constData(), 4);
        revMunit = (MsgUnit*)malloc(size);
        revMunit->totalLen = size;
        waitSize = size - 4;
    }

    QByteArray tmp = socket->readAll();
    buffer.append(tmp);
    tmp.clear();
    if (buffer.size() >= waitSize)
    {
        tmp = buffer.first(waitSize);
        buffer.remove(0, waitSize);
        memcpy((char*)revMunit + 4, tmp.constData(), waitSize);
        waitSize = 0;
        std::shared_ptr<MsgUnit> sptr(revMunit, [](MsgUnit* munit){
            try {
                free(munit);
            } catch (const std::exception& e) {
                qDebug() << e.what();
            }
        });
        emit newMunit(sptr);
        revMunit = nullptr;
    }
//    qDebug() << "main end" << now;
}

