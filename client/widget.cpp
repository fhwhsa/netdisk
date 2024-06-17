#include "widget.h"
#include "msgUnit.h"
#include "ui_widget.h"

#include <QSettings>
#include <QDebug>
#include <QHostAddress>
#include <QMessageBox>
#include <string>
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
    QString name = ui->userName->text();
    QString passwd = ui->passwd->text();

    QString msg = QString("name:%1\r\npasswd:%2\r\n").arg(name).arg(passwd);
    std::string sstr = msg.toStdString();
    const char* cstr = sstr.c_str();
    size_t num = strlen(cstr);
    MsgUnit* munit = MsgUnit::make_dataunit(MsgType::MSG_TYPE_LOGIN_REQUEST, num, cstr);
    socket->write((char*)munit, munit->totalLen);
    free(munit);
    munit = nullptr;
}

void Widget::regist()
{

}

//void Widget::send()
//{
//    QString str = ui->lineEdit->text();
//    if (str.isEmpty())
//        QMessageBox::warning(this, "warn", "null");
//    else
//    {
//        std::string sstr = str.toStdString();
//        const char* cstr = sstr.c_str();
//        size_t num = strlen(cstr);
//        MsgUnit* munit = MsgUnit::make_dataunit(MsgType::MSG_TYPE_LOGIN_REQUEST, num, cstr);
//        socket->write((char*)munit, munit->totalLen);
//        free(munit);
//        munit = NULL;
////        qDebug() << "write to server";
//    }
//}


