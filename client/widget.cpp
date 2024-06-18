#include "widget.h"
#include "msgUnit.h"
#include "ui_widget.h"

#include <QSettings>
#include <QDebug>
#include <QHostAddress>
#include <QMessageBox>
#include <string>
#include <string.h>
#include <QEventLoop>
#include <QTimer>
#include <QByteArray>
#include <string.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    init();
    iniSignalSlots();
}

Widget::~Widget()
{
    delete ui;

    if (socket)
    {
        socket->disconnectFromHost();
        socket->waitForDisconnected();
    }
}

void Widget::init()
{
    loadconfig();
    if (connectServer())
        qDebug() << "Connected!";
    else
        qDebug() << "Failed to connect server!";    
}

void Widget::iniSignalSlots()
{
    connect(ui->btnRegist, SIGNAL(clicked()), this, SLOT(regist()));
    connect(ui->btnLogin, SIGNAL(clicked()), this, SLOT(login()));
}

void Widget::loadconfig()
{
    QSettings ini = QSettings(":/config/res/config.ini", QSettings::IniFormat);
    ini.beginGroup("ADDRESS");
    ip = ini.value("IPADDR").toString();
    port = ini.value("PORT").toString();
//    qDebug() << ip << port;
    ini.endGroup();
}

bool Widget::connectServer()
{
    socket = new QTcpSocket(this);
    socket->connectToHost(QHostAddress(ip), port.toUInt());
    return socket->waitForConnected();
}

void Widget::login()
{
    QString email = ui->userName->text();
    QString passwd = ui->passwd->text();

    QString msg = QString("email:%1\r\npasswd:%2\r\n").arg(email).arg(passwd);
    std::string sstr = msg.toStdString();
    const char* cstr = sstr.c_str();
    size_t num = strlen(cstr);
    MsgUnit* munit = MsgUnit::make_dataunit(MsgType::MSG_TYPE_LOGIN_REQUEST, num, cstr);
    socket->write((char*)munit, munit->totalLen);

    free(munit);
    munit = nullptr;

    // 等待响应
    bool isGetRespond = false;  // 是否得到响应的标志
    QEventLoop loop;
    QTimer timer;
    QMetaObject::Connection connection;
    connection = connect(socket, &QTcpSocket::readyRead, [&timer, &loop, &munit, &isGetRespond, &connection, this](){
        timer.stop();
        disconnect(connection);
        QByteArray data = socket->readAll();
        size_t totalLen = data.size();
        munit = (MsgUnit*)malloc(totalLen);
        memcpy(munit, data.constData(), totalLen);
//        qDebug() << (char*)munit->msg;
        isGetRespond = true;
        loop.quit();
    });
    connect(&timer, &QTimer::timeout, [&loop, &connection, this](){
        disconnect(connection);
        loop.quit();
        QMessageBox::critical(this, "Error", "Timeout!");
    });
    timer.start(3000);
    loop.exec();

    if (isGetRespond)
    {
        QString respond = QString((char*)munit->msg);
        if (respond == "success\r\n")   // 登陆逻辑
        {
            QMessageBox::information(this, "info", "login success!");
        }
        else
        {
            QMessageBox::critical(this, "Error", "请检查你的登陆邮箱和密码!");
        }
    }
}

void Widget::regist()
{
}


