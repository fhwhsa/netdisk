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
    // 检查任务数限制
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
    connect(piw, &ProgressItemWidget::cancel, uw, &UploadWorker::cancel);
    connect(uw, &UploadWorker::updateProgress, this, [piw](qint64 val){
        QMetaObject::invokeMethod(piw, [piw, val](){
                piw->updateFinshSize(val);
            }, Qt::QueuedConnection);
    });
    connect(uw, &UploadWorker::workFinsh, [this](int status, int taskId, QString errorMsg){
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

        if (0 == status) // 上传完成
        {
            // 添加到传输完成队列
            ui->finshList->addItem(QString("上传：%1").arg(piw->getFileName()));
            ui->uploadList->takeItem(row);
//            delete piw;
        }
        else if (-1 == status) // 出错
        {
            piw->setErrorMsg(errorMsg);
        }
        else if (2 == status) // 取消
        {
            ui->uploadList->takeItem(row);
        }
        uploadWorkerList.erase(uploadWorkerList.find(taskId));
    });
    uw->start();
}

void TransmitPage::addDownloadTask(QString filepath, qint64 filesize)
{
    // 检查任务数限制
    if (downloadWorkerList.size() >= maxDownloadTaskNum)
    {
        BubbleTips::showBubbleTips("当前下载任务数已达到最大", 2, this);
        return;
    }

    // 生成标识id
    QRandomGenerator* randomGenerator = QRandomGenerator::global();
    int tid;
    do
    {
        tid = randomGenerator->bounded(100);
    } while (downloadWorkerList.find(tid) != downloadWorkerList.end());

    // ui初始化
    QString filename = filepath.mid(filepath.lastIndexOf('/') + 1);
    ProgressItemWidget* piw = new ProgressItemWidget(tid, filename, filesize);
    QListWidgetItem* item = new QListWidgetItem(ui->downloadList);
    ui->downloadList->setItemWidget(item, piw);
    item->setSizeHint(QSize(0, piw->height()));

    // 获取下载路径
    QSettings ini = QSettings("./setting.ini", QSettings::IniFormat);
    ini.beginGroup("DOWNLOADSETTING");
    QString path = ini.value("PATH").toString();
    ini.endGroup();

    // 创建任务
    DownloadWorker* dw = new DownloadWorker(tid, filepath, filesize, path);
    downloadWorkerList.insert(tid, dw);
    connect(dw, &DownloadWorker::updateProgress, this, [piw](qint64 val){
        QMetaObject::invokeMethod(piw, [piw, val](){
                piw->updateFinshSize(val);
            }, Qt::QueuedConnection);
    });
    connect(piw, &ProgressItemWidget::cancel, dw, &DownloadWorker::cancel);
    connect(dw, &DownloadWorker::workFinsh, [this](int status, int taskId, QString msg){
        int row;
        ProgressItemWidget* piw = nullptr;
        for (row = ui->downloadList->count() - 1; ~row; --row)
        {
            QListWidgetItem* item = ui->downloadList->item(row);
            ProgressItemWidget* _piw = static_cast<ProgressItemWidget*>(ui->downloadList->itemWidget(item));
            if (taskId == _piw->getTaskId())
            {
                piw = _piw;
                break;
            }
        }

        if (0 == status)
        {
            ui->finshList->addItem(QString("下载：%1").arg(piw->getFileName()));
            ui->downloadList->takeItem(row);
        }
        else if (-1 == status)
        {
            piw->setErrorMsg(msg);
        }
        else if (2 == status)
        {
            ui->downloadList->takeItem(row);
            // 删除文件
            QFile file(msg);
            file.remove();
        }
        downloadWorkerList.erase(downloadWorkerList.find(taskId));
    });
    dw->start();
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
    isCancel(false),
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
    return true;
}

void UploadWorker::run()
{
    QTcpSocket* socket = nullptr;
    QMetaObject::Connection conn1, conn2;
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
            emit workFinsh(-1, tid, socket->errorString());
            break;
        }

        // 绑定槽函数
        conn1 = connect(socket, &QTcpSocket::readyRead, [this, socket](){
            upload(socket);
        });
        conn2 = connect(this, &UploadWorker::cancel, [this, socket](){
            isCancel = true;
//            if (socket->bytesAvailable() == 0)
//                upload(socket);
        });

        // 发送start指令，并等待响应
        MsgUnit* munit = MsgTools::generateUploadFileStartRequest(filename, diskpath);
        socket->write((char*)munit, munit->totalLen);
        if (!socket->waitForBytesWritten())
        {
            emit workFinsh(-1, tid, socket->errorString());
            break;
        }

        QEventLoop loop;
        connect(this, &UploadWorker::workFinsh, [&loop](){
            loop.quit();
        });
        loop.exec();
    } while (0);

    disconnect(conn1);
    disconnect(conn2);
    if (nullptr != socket && socket->state() == QAbstractSocket::ConnectedState)
    {
        socket->close();
        delete socket;
        socket = nullptr;
    }
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

    if (isCancel)
    {
        // 发送取消任务请求
        MsgUnit* req = MsgTools::generateUploadFileCancelRequest();
        socket->write((char*)req, req->totalLen);
        if (!socket->waitForBytesWritten(5000))
        {
            qDebug() << "请重试取消";
        }
        free(req);
        isCancel = false; // 让其接下来能正常接收取消任务的响应
    }
    else if (munit->msgType == MsgType::MSG_TYPE_UPLOADFILE_FINSH_RESPOND)
    {
        if ("recv" == recvList[0])
        {
            emit workFinsh(0, tid);
        }
        else
        {
            emit workFinsh(-1, tid, getStatusCodeString(recvList[1].mid(7)));
        }
    }

    else if (munit->msgType == MsgType::MSG_TYPE_UPLOADFILE_DATA_RESPOND || munit->msgType == MsgType::MSG_TYPE_UPLOADFILE_START_RESPOND)
    {
        // 获取服务器读取的字节数
        if (3 != recvList.size())
        {
            emit workFinsh(-1, tid, "数据传输错误");
        }
        if ("failure" == recvList[0])
        {
            emit workFinsh(-1, tid, getStatusCodeString(recvList[1].mid(7)));
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
                emit workFinsh(-1, tid, socket->errorString());
            }
        }
        else
        {
            file.close();
            MsgUnit* req = MsgTools::generateUploadFileFinshRequest();
            socket->write((char*)req, req->totalLen);
            if (!socket->waitForBytesWritten(5000))
            {
                emit workFinsh(-1, tid, socket->errorString());
            }
        }
    }

    else if (munit->msgType == MsgType::MSG_TYPE_UPLOADFILE_CANCEL_RESPOND)
    {
        if (recvList[0] == "recv")
        {
            emit workFinsh(2, tid);
        }
        else
            BubbleTips::showBubbleTips("请重试(" + getStatusCodeString(recvList[1].mid(7)) + "）", 2);
    }

    free(munit);
}

DownloadWorker::DownloadWorker(int _tid, QString _downloadPath, qint64 _fileSize, QString _storePath) :
    tid(_tid),
    downloadPath(_downloadPath),
    fileSize(_fileSize),
    isCancel(false),
    storePath(_storePath)
{
    filename = downloadPath.mid(downloadPath.lastIndexOf("/") + 1);
    downloadSize = 0;
}

void DownloadWorker::download(QTcpSocket* socket)
{
    QByteArray data = socket->peek(4);
    uint totalLen;
    memcpy(&totalLen, data.constData(), 4);
    MsgUnit* munit = (MsgUnit*)malloc(totalLen);
    QByteArray allData = socket->readAll();
    memcpy(munit, allData.constData(), totalLen);

    if (isCancel)
    {
        // 发送取消任务请求
        MsgUnit* req = MsgTools::generateDownloadFileCancelRequest();
        socket->write((char*)req, req->totalLen);
        if (!socket->waitForBytesWritten(5000))
        {
            qDebug() << "请重试取消";
        }
        free(req);
        isCancel = false; // 让其接下来能正常接收取消任务的响应
    }

    else if (munit->msgType == MsgType::MSG_TYPE_DOWNLOADFILE_FAILURE_RESPOND)
    {
        QString recvContent((char*)munit->msg);
        QStringList recvList = recvContent.split("\r\n");
        if (recvList.size() != 2)
            emit workFinsh(-1, tid, "数据传输错误");
        else
            emit workFinsh(-1, tid, getStatusCodeString(recvList[0].mid(7)));
    }

    else if (munit->msgType == MsgType::MSG_TYPE_DOWNLOADFILE_START_RESPOND)
    {
        QString recvContent((char*)munit->msg);
        QStringList recvList = recvContent.split("\r\n");
        if (recvList.size() != 3)
            emit workFinsh(-1, tid, "数据传输错误");
        else
            fileSize = recvList[1].toLong();

        MsgUnit* req = MsgTools::generateDownloadFileDataRequest(downloadSize);
        socket->write((char*)req, req->totalLen);
        if (!socket->waitForBytesWritten(5000))
            emit workFinsh(-1, tid, socket->errorString());
        free(req);
    }

    else if (munit->msgType == MsgType::MSG_TYPE_DOWNLOADFILE_DATA_RESPOND)
    {
        qint64 writeBytes = file.write((char*)munit->msg, munit->msgLen - 1);
        if (-1 == writeBytes)
            emit workFinsh(-1, tid, file.errorString());
        else
        {
            downloadSize += writeBytes;
            emit updateProgress(writeBytes);

            // 继续请求数据
            if (downloadSize < fileSize)
            {
                MsgUnit* req = MsgTools::generateDownloadFileDataRequest(downloadSize);
                socket->write((char*)req, req->totalLen);
                if (!socket->waitForBytesWritten(5000))
                    emit workFinsh(-1, tid, socket->errorString());
                free(req);
            }

            // 下载完成
            else
            {
                emit workFinsh(0, tid);
                file.close();
            }
        }
    }

    else if (munit->msgType == MsgType::MSG_TYPE_DOWNLOADFILE_CANCEL_RESPOND)
    {
        QString recvContent((char*)munit->msg);
        QStringList recvList = recvContent.split("\r\n");
        if (recvList[0] == "recv")
            emit workFinsh(2, tid, storePath + "/" + filename);
        else
            BubbleTips::showBubbleTips("请重试(" + getStatusCodeString(recvList[1].mid(7)) + "）", 2);
    }

    free(munit);
}

void DownloadWorker::run()
{
    QTcpSocket* socket = nullptr;
    QMetaObject::Connection conn1, conn2;
    do
    {
        QString storeFilePath = storePath + "/" + filename;
        // 打开文件
        if (QFile::exists(storeFilePath))
        {
            emit workFinsh(-1, tid, "文件已下载");
            break;
        }
        file.setFileName(storeFilePath);
        if (!file.open(QIODevice::WriteOnly))
        {
            emit workFinsh(-1, tid, "文件打开失败");
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
            emit workFinsh(-1, tid, socket->errorString());
            break;
        }

        // 绑定槽函数
        conn1 = connect(socket, &QTcpSocket::readyRead, [this, socket](){
            download(socket);
        });
        conn2 = connect(this, &DownloadWorker::cancel, [this]{
            isCancel = true;
        });

        // 发送start指令，并等待响应
        MsgUnit* munit = MsgTools::generateDownloadFileStartRequest(downloadPath);
        socket->write((char*)munit, munit->totalLen);
        if (!socket->waitForBytesWritten())
        {
            emit workFinsh(-1, tid, socket->errorString());
            break;
        }

        QEventLoop loop;
        connect(this, &DownloadWorker::workFinsh, [&loop](){
            loop.quit();
        });
        loop.exec();

        qDebug() << "loop exit";
    } while (0);

    disconnect(conn1);
    disconnect(conn2);
    if (nullptr != socket && socket->state() == QAbstractSocket::ConnectedState)
    {
        socket->close();
        delete socket;
        socket = nullptr;
    }
}


