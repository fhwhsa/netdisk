#include "widget.h"
#include "dataUnit.h"
#include "ui_widget.h"

#include <QSettings>
#include <QDebug>
#include <QHostAddress>
#include <QMessageBox>

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
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(send()));
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

void Widget::send()
{
    QString str = ui->lineEdit->text();
    if (str.isEmpty())
        QMessageBox::warning(this, "warn", "null");
    else
    {
        DataUnit* dunit = DataUnit::make_dataunit(DataUnit::DATA_TYPE_TEXT, str.size(), str.toStdString().c_str());
        socket->write((char*)dunit, dunit->totalLen);
        free(dunit);
        dunit = NULL;
        qDebug() << "write to server";
    }
}


