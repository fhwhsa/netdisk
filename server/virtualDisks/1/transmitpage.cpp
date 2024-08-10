#include "transmitpage.h"
#include "ui_transmitpage.h"

#include "msgtools.h"
#include "bubbletips.h"
#include "respondwatcher.h"
#include "msgUnit.h"
#include "statusCode.h"

#include <QString>
#include <QListWidgetItem>
#include <QSize>
#include <QRandomGenerator>
#include <QSettings>
#include <QByteArray>
#include <QDebug>

TransmitPage::TransmitPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TransmitPage)
{
    ui->setupUi(this);
    init();
    iniSignalSlots();
}

TransmitPage::~TransmitPage()
{
    // 关闭上传任务，将上传任务保存到数据库
    for (int i = ui->uploadList->count() - 1; ~i; --i)
    {
        ProgressItemWidget* piw = static_cast<ProgressItemWidget*>(ui->uploadList->itemWidget(ui->uploadList->item(i)));
        int taskId = piw->getTaskId();
        delete piw;
        delete uploadTaskList[taskId];
    }

    delete ui;
    qDebug() << "transmitpage delete";
}

void TransmitPage::addUploadTask(QString filepath, QString diskPath)
{
    if (static_cast<int>(uploadTaskList.size()) >= maxUploadTaskNum)
    {
        BubbleTips::showBubbleTips("已达到最大上传任务数", 2, this);
        return;
    }

    QListWidgetItem* item = new QListWidgetItem(ui->uploadList);
    ProgressItemWidget* piw = new ProgressItemWidget();
    ui->uploadList->setItemWidget(item, piw);
    item->setSizeHint(QSize(ui->uploadList->width(), piw->height()));

    int taskId = QRandomGenerator::global()->bounded(100, 1000);
    while (uploadTaskList.find(taskId) != uploadTaskList.end())
    {
        taskId = QRandomGenerator::global()->bounded(100, 1000);
    }

    UploadWorker* uw = new UploadWorker(taskId, filepath, diskPath, piw);
    connect(uw, &UploadWorker::taskFinsh, [this](int taskId){ // 任务完成释放资源，并添加到传输完成列
        qDebug() << "taskfinsh";
        for (int i = ui->uploadList->count() - 1; ~i; --i)
        {
            ProgressItemWidget* piw = static_cast<ProgressItemWidget*>(ui->uploadList->itemWidget(ui->uploadList->item(i)));
            if (taskId == piw->getTaskId())
            {
                ui->finshList->addItem(piw->getFileName());
                delete piw;
                ui->uploadList->takeItem(i);
                uploadTaskList.erase(uploadTaskList.find(taskId));

                break;
            }
        }
    });
    uploadTaskList.insert(taskId, uw);

    uw->start();
}

void TransmitPage::init()
{
    QString style = "QPushButton { background-color: transparent; border: none; }";
    ui->downloadListBtn->setStyleSheet(style);
    ui->finshListBtn->setStyleSheet(style);
    ui->uploadListBtn->setStyleSheet(style);

    ui->uploadList->hide();
    ui->downloadList->hide();
    currListWidget = ui->finshList;
}

void TransmitPage::iniSignalSlots()
{
    connect(ui->uploadListBtn, &QPushButton::clicked, this, &TransmitPage::clickUploadListBtn);
    connect(ui->downloadListBtn, &QPushButton::clicked, this, &TransmitPage::clickDownloadListBtn);
    connect(ui->finshListBtn, &QPushButton::clicked, this, &TransmitPage::clickFinshListBtn);
}

void TransmitPage::getMsg(std::shared_ptr<MsgUnit> sptr)
{
    Q_UNUSED(sptr);
}

void TransmitPage::clickUploadListBtn()
{
    currListWidget->hide();
    ui->uploadList->show();
    currListWidget = ui->uploadList;
}

void TransmitPage::clickDownloadListBtn()
{
    currListWidget->hide();
    ui->downloadList->show();
    currListWidget = ui->downloadList;

}

void TransmitPage::clickFinshListBtn()
{
    currListWidget->hide();
    ui->finshList->show();
    currListWidget = ui->finshList;

}





UploadWorker::UploadWorker(int _taskId, QString filepath, QString diskPath, ProgressItemWidget *_piw) :
    piw(_piw),
    taskId(_taskId)
{
    // 接收按钮信号控制传输过程
    connect(piw, &ProgressItemWidget::cancel, this, &UploadWorker::cancelTask);
    connect(piw, &ProgressItemWidget::pause, this, &UploadWorker::pauseTask);
    connect(piw, &ProgressItemWidget::cont, this, &UploadWorker::contTask);

    // 连接服务器
    QSettings ini = QSettings(":/config/Settings::IniFormat);
    ini.beginGroup("ADDRESS");
    QString ip = ini.value("IPADDR").toString();
    QString port = ini.value("PORT").toString();
    ini.endGroup();
    socket = new QTcpSocket(this);
    socket->connectToHost(QHostAddress(ip), port.toUInt());
    if (!socket->waitForConnected())
    {
        piw->setErrorMsg("连接服务器失败");
        return;
    }

    // 初始化文件资源
    file = new QFile(filepath);
    if (!file->open(QIODevice::ReadOnly))
    {
        piw->setErrorMsg("文件打开失败");
        return;
    }

    QString filename = filepath.mid(filepath.lastIndexOf('/') + 1);
    piw->init(taskId, filename, file->size());

    // 发送start
    MsgUnit* munit = MsgTools::generateUploadFileRequest_start(filename, diskPath);
    if (-1 == socket->write((char*)munit, munit->totalLen))
        piw->setErrorMsg(socket->errorString());
    delete munit;
    munit = nullptr;
}

UploadWorker::~UploadWorker()
{
    qDebug() << "worker delete";
    releaseSources();
}

void UploadWorker::releaseSources()
{
    if (nullptr != socket)
    {
        socket->close();
        socket->waitForDisconnected();
        delete socket;
        socket = nullptr;
    }
    if (nullptr != file)
    {
        file->close();
        delete file;
        file = nullptr;
    }
}

void UploadWorker::recvHandler()
{
    QByteArray data = socket->readAll();
    size_t totalLen = data.size();
    MsgUnit* munit = (MsgUnit*)malloc(totalLen);
    memcpy(munit, data.constData(), totalLen);

    // 处理信息
    QStringList msg = MsgTools::getAllRows(munit);

    // 发送数据
    if ("recv" == msg[0])
    {
        if (msg.size() == 3)
        {
            piw->updateFinshSize(msg[1].toLongLong());
        }
        // 发送内容
        QByteArray data = file->read(4096);
        MsgUnit* sendMunit = nullptr;
        if (0 == data.size())
            sendMunit = MsgTools::generateUploadFileRequest_finsh();
        else
            sendMunit = MsgTools::generateUploadFileRequest_next(data);

        if (-1 == socket->write((char*)sendMunit, sendMunit->totalLen))
        {
            piw->setErrorMsg(socket->errorString());
        }
        if (nullptr != sendMunit)
        {
            delete sendMunit;
            sendMunit = nullptr;
        }
    }

    else if ("finsh" == msg[0]) // 上传完成，释放资源，结束线程
    {
        connect(this, &QThread::finished, [this](){
            emit taskFinsh(this->taskId);
        });
        emit exitEventLoop();
    }

    else // failure
    {
        piw->setErrorMsg(getStatusCodeString(MsgTools::getRow(munit, 1)));
        // 后续处理
    }

    free(munit);
    munit = nullptr;
}

void UploadWorker::cancelTask()
{

}

void UploadWorker::pauseTask()
{

}

void UploadWorker::contTask()
{

}

void UploadWorker::run()
{
    QEventLoop loop;
    QMetaObject::Connection conn = connect(piw, &ProgressItemWidget::pause, [&loop](){
        loop.quit();
    });
    connect(this, &UploadWorker::exitEventLoop, [&loop](){
        loop.quit();
    });
    connect(socket, &QTcpSocket::readyRead, this, &UploadWorker::recvHandler);
    loop.exec();
    disconnect(conn);
}
