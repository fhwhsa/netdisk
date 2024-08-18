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
#include <QSettings>
#include <QByteArray>
#include <QFileInfo>
#include <QDebug>
#include <QFileInfo>
#include <string.h>
#include <QEventLoop>
#include <QRandomGenerator>

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
    delete ui;
}

void TransmitPage::addUploadTask(QString filepath, QString diskPath)
{
    if (uploadWorkerList.size() >= maxUploadTaskNum)
    {
        BubbleTips::showBubbleTips("当前上传任务数已达到最大", 2, this);
        return;
    }

    // 生成标识id
    QRandomGenerator* randomGenerator = QRandomGenerator::global();
    int tid;
    do
    {
        tid = randomGenerator->bounded(100);
    } while (uploadWorkerList.find(tid) != uploadWorkerList.end());

    // ui初始化
    QFileInfo finfo = QFileInfo(filepath);
    ProgressItemWidget* piw = new ProgressItemWidget(tid, finfo.fileName(), finfo.size());
    QListWidgetItem* item = new QListWidgetItem(ui->uploadList);
    ui->uploadList->setItemWidget(item, piw);
    item->setSizeHint(QSize(0, piw->height()));

    // 创建任务
    UploadWorker* uw = new UploadWorker(tid, filepath, diskPath);
    uploadWorkerList.insert(tid, uw);
    connect(uw, &UploadWorker::updateProgress, this, [piw](qint64 val){
        QMetaObject::invokeMethod(piw, [piw, val](){
                piw->updateFinshSize(val);
            }, Qt::QueuedConnection);
    });
    connect(uw, &UploadWorker::workFinsh, [this](bool isSuccess, int taskId, QString errorMsg){
        int row;
        ProgressItemWidget* piw = nullptr;
        for (row = ui->uploadList->count() - 1; ~row; --row)
        {
            QListWidgetItem* item = ui->uploadList->item(row);
            ProgressItemWidget* _piw = static_cast<ProgressItemWidget*>(ui->uploadList->itemWidget(item));
            if (taskId == _piw->getTaskId())
            {
                piw = _piw;
                break;
            }
        }

        if (isSuccess)
        {
            // 添加到传输完成队列
            ui->finshList->addItem(piw->getFileName());
            ui->uploadList->takeItem(row);
//            delete piw;
        }
        else
        {
            piw->setErrorMsg(errorMsg);
        }
        uploadWorkerList.erase(uploadWorkerList.find(taskId));
    });
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

UploadWorker::UploadWorker(int _tid, QString _filepath, QString _diskPath)
    : filepath(_filepath),
    diskpath(_diskPath),
    tid(_tid)
{
    filename = filepath.mid(filepath.lastIndexOf('/') + 1);
}

bool UploadWorker::initFile()
{
    file.setFileName(filepath);
    if (!file.open(QIODevice::ReadOnly))
    {
        emit workFinsh(false, tid, "文件打开失败");
        return false;
    }

    QFileInfo fileinfo(file);
    fileSize = fileinfo.size();
    uploadSize = 0;
//    piw->init(filename, fileSize);
    return true;
}

void UploadWorker::run()
{
    QTcpSocket* socket = nullptr;
    QMetaObject::Connection conn;
    do
    {
        // 打开文件
        if (!initFile())
        {
            break;
        }

        // 连接服务器
        QSettings ini = QSettings(":/config/res/config.ini", QSettings::IniFormat);
        ini.beginGroup("ADDRESS");
        QString ip = ini.value("IPADDR").toString();
        QString port = ini.value("PORT").toString();
        ini.endGroup();
        socket = new QTcpSocket();
        socket->connectToHost(QHostAddress(ip), port.toUInt());
        if (!socket->waitForConnected(5000))
        {
            emit workFinsh(false, tid, socket->errorString());
            break;
        }

        // 绑定槽函数
        conn = connect(socket, &QTcpSocket::readyRead, [this, socket](){
            upload(socket);
        });

        // 发送start指令，并等待响应
        MsgUnit* munit = MsgTools::generateUploadFileStartRequest(filename, diskpath);
        socket->write((char*)munit, munit->totalLen);
        if (!socket->waitForBytesWritten())
        {
            emit workFinsh(false, tid, socket->errorString());
            break;
        }

        QEventLoop loop;
        connect(this, &UploadWorker::workFinsh, [&loop](){
            loop.quit();
        });
        loop.exec();
    } while (0);

    disconnect(conn);
    socket->close();
    delete socket;
    socket = nullptr;
}

void UploadWorker::upload(QTcpSocket* socket)
{
    QByteArray data = socket->peek(4);
    uint totalLen;
    memcpy(&totalLen, data.constData(), 4);
    MsgUnit* munit = (MsgUnit*)malloc(totalLen);
    QByteArray allData = socket->readAll();
    memcpy(munit, allData.constData(), totalLen);
    QString recvContent((char*)munit->msg);
    QStringList recvList = recvContent.split("\r\n");

    if (munit->msgType == MsgType::MSG_TYPE_UPLOADFILE_FINSH_RESPOND)
    {
        if (recvList.size() > 0 && "recv" == recvList[0])
        {
            emit workFinsh(true, tid);
        }
        else
        {
            emit workFinsh(false, tid, getStatusCodeString(recvList[1]));
        }
        free(munit);
        return;
    }

    free(munit);

    // 获取服务器读取的字节数
    if (3 != recvList.size())
    {
        emit workFinsh(false, tid, "数据传输错误");
        return;
    }
    if ("failure" == recvList[0])
    {
        emit workFinsh(false, tid, getStatusCodeString(recvList[1]));
        return;
    }
    qint64 serverRecv = recvList[1].toLong();

    // 更新进度
    uploadSize += serverRecv;
    emit updateProgress(serverRecv);

    // 发送数据
    if (uploadSize < fileSize)
    {
        char buffer[blockSize];
        qint64 res = file.read(buffer, blockSize);
        MsgUnit* req = MsgTools::generateUploadFileDataRequest(res, buffer);
        socket->write((char*)req, req->totalLen);
        if (!socket->waitForBytesWritten(5000))
        {
            emit workFinsh(false, tid, socket->errorString());
            free(req);
            return;
        }
        free(req);
    }
    else
    {
        file.close();
        MsgUnit* req = MsgTools::generateUploadFileFinshRequest();
        socket->write((char*)req, req->totalLen);
        if (!socket->waitForBytesWritten(5000))
        {
            emit workFinsh(false, tid, socket->errorString());
            free(req);
            return;
        }
        free(req);
    }
}
