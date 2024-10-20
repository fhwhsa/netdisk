#include "login.h"
#include "ui_login.h"
#include "msgUnit.h"
#include "msgtools.h"
#include "statusCode.h"

#include <QSettings>
#include <QDebug>
#include <QHostAddress>
#include <QMessageBox>
#include <QEventLoop>
#include <QTimer>
#include <QByteArray>
#include <QSize>
#include <string.h>
#include <QAbstractSocket>

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    init();
    iniSignalSlots();
}

Login::~Login()
{
    delete ui;

    if (mainPage)
        delete mainPage;

    if (socket)
    {
        socket->close();
        if (socket->state() != QAbstractSocket::UnconnectedState)
            socket->waitForDisconnected();
        delete socket;
        socket = nullptr;
    }
}

bool Login::init()
{
    this->setMinimumSize(QSize(1100, 700));
    this->setWindowTitle(" ");
    loadconfig();
    socket = nullptr;
    mainPage = nullptr;
    return true;
}

void Login::iniSignalSlots()
{
    connect(ui->btnRegist, SIGNAL(clicked()), this, SLOT(regist()));
    connect(ui->btnLogin, SIGNAL(clicked()), this, SLOT(login()));
}

void Login::loadconfig()
{
    QSettings ini = QSettings(":/config/res/config.ini", QSettings::IniFormat);
    ini.beginGroup("ADDRESS");
    ip = ini.value("IPADDR").toString();
    port = ini.value("PORT").toString();
    ini.endGroup();
}

bool Login::connectServer()
{
    if (nullptr != socket)
    {
        delete socket;
        socket = nullptr;
    }
    socket = new QTcpSocket(this);
    socket->connectToHost(QHostAddress(ip), port.toUInt());
    return socket->waitForConnected();
}

bool Login::tryConnect()
{
    bool flag = false;
    do
    {
        flag = false;
        if (connectServer())
        {
            qDebug() << "Connected!";
            return true;
        }
        else
        {
            qDebug() << socket->errorString();
            if (QMessageBox::Yes == QMessageBox::question(this, " ", socket->errorString() + QString("\nretry?"), QMessageBox::Yes | QMessageBox::No))
                flag = true;
            else
                return false;
        }
    } while (flag);

    return false;
}

void Login::login()
{
    // if ((nullptr == socket || socket->state() != QAbstractSocket::ConnectedState) && !tryConnect())
    //     return;
    if (!tryConnect())
        return;

    QString email = ui->userName->text();
    QString passwd = ui->passwd->text();

    if (email.isEmpty() || passwd.isEmpty())
    {
        QMessageBox::critical(this, "登陆", "邮箱和密码不能为空！");
        return;
    }

    // 发送登陆请求
    MsgUnit* munit = MsgTools::generateLoginRequest(email, passwd);
    socket->write((char*)munit, munit->totalLen);
    free(munit);
    munit = nullptr;

    // 等待响应
    bool isGetRespond = false;  // 是否得到响应的标志
    QEventLoop loop(this);
    QTimer timer(this);
    QMetaObject::Connection connection;
    connection = connect(socket, &QTcpSocket::readyRead, [&timer, &loop, &munit, &isGetRespond, &connection, this](){
        timer.stop();
        disconnect(connection); // 取消槽函数绑定

        // 读取消息体
        QByteArray data = socket->readAll();
        size_t totalLen = data.size();
        munit = (MsgUnit*)malloc(totalLen);
        memcpy(munit, data.constData(), totalLen);
        isGetRespond = true;
        loop.quit();
    });
    connect(&timer, &QTimer::timeout, [&loop, &connection, this](){
        disconnect(connection);
        loop.quit();
        QMessageBox::critical(this, "登陆", "Timeout! " + socket->errorString());
        qDebug() << socket->state();

        // 断线重连机制
        if (QAbstractSocket::UnconnectedState == socket->state())
        {
            delete socket;
            socket = nullptr;
        }
    });
    timer.start(3000); // 设置超时时间为3s
    loop.exec(); // 开启事件循环，等待socket可读信号或者超时信号触发

    // 处理登陆结果
    if (isGetRespond)
    {
        QString respond = QString((char*)munit->msg);
        if (respond.left(7) == "success")
        {
            QString userId = respond.split("\r\n")[1].mid(3);
            mainPage = new MainPage(userId, email, socket);
            connect(mainPage, &MainPage::mainPageClosed, [this, &connection](){
                delete mainPage;
                mainPage = nullptr;
                this->show();
            });
            this->hide();
            mainPage->show();
        }
        else
        {
            QString statusCode = MsgTools::getRow(munit, 1);
            if (statusCode.length() <= 7)
                QMessageBox::critical(this, "登陆", "通信错误");
            else
                QMessageBox::critical(this, "登陆", getStatusCodeString(statusCode.mid(7)));
        }

        free(munit);
        munit = nullptr;
    }
}

void Login::regist()
{
}
