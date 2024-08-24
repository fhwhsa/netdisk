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

void TransmitPage::uploadWorkFinshHandler(int status, int taskId, QString errorMsg)
{
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
        delete ui->uploadList->takeItem(row);
//        delete piw;   uploadList经过hide，show函数后piw执行析构函数？？
    }
    else if (-1 == status) // 出错
    {
        piw->setErrorMsg(errorMsg);
    }
    else if (2 == status) // 取消
    {
        ui->uploadList->takeItem(row);
    }
    else if (1 == status) // 暂停
    {
//      qDebug() << "erase uploadworker";
    }
    uploadWorkerList.erase(uploadWorkerList.find(taskId));
}

void TransmitPage::downloadWorkFinshHandler(int status, int taskId, QString msg)
{
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
    else if (1 == status)
    {
//        qDebug() << "worker exit";
    }
    downloadWorkerList.erase(downloadWorkerList.find(taskId));
}

void TransmitPage::addUploadTask(QString filepath, QString diskPath, WorkType wt)
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
    UploadWorker* uw = new UploadWorker(tid, filepath, diskPath, wt);
    uploadWorkerList.insert(tid, uw);
    connect(piw, &ProgressItemWidget::cancel, uw, &UploadWorker::cancel);
    connect(piw, &ProgressItemWidget::pause, uw, &UploadWorker::pause);
    connect(piw, &ProgressItemWidget::cont, [this, piw, filepath, diskPath]() {
        for (int row = ui->uploadList->count() - 1; ~row; --row)
        {
            QListWidgetItem* item = ui->uploadList->item(row);
            ProgressItemWidget* _piw = static_cast<ProgressItemWidget*>(ui->uploadList->itemWidget(item));
            if (piw->getTaskId() == _piw->getTaskId())
            {
                ui->uploadList->takeItem(row);
                break;
            }
        }
        addUploadTask(filepath, diskPath, WorkType::CONTINUE_WORK);
    });
    connect(uw, &UploadWorker::updateProgress, this, [piw](qint64 val){
        QMetaObject::invokeMethod(piw, [piw, val](){
                piw->updateFinshSize(val);
            }, Qt::QueuedConnection);
    });
    connect(uw, &UploadWorker::workFinsh, [this](int status, int taskId, QString errorMsg){
        uploadWorkFinshHandler(status, taskId, errorMsg);
    });
    uw->start();
}

void TransmitPage::addDownloadTask(QString filepath, qint64 filesize, WorkType wt)
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
    DownloadWorker* dw = new DownloadWorker(tid, filepath, filesize, path, wt);
    downloadWorkerList.insert(tid, dw);
    connect(dw, &DownloadWorker::updateProgress, this, [piw](qint64 val){
        QMetaObject::invokeMethod(piw, [piw, val](){
                piw->updateFinshSize(val);
            }, Qt::QueuedConnection);
    });
    connect(piw, &ProgressItemWidget::cancel, dw, &DownloadWorker::cancel);
    connect(piw, &ProgressItemWidget::pause, dw, &DownloadWorker::pause);
    connect(piw, &ProgressItemWidget::cont, [this, piw, filepath, filesize](){
        for (int row = ui->downloadList->count() - 1; ~row; --row)
        {
            QListWidgetItem* item = ui->downloadList->item(row);
            ProgressItemWidget* _piw = static_cast<ProgressItemWidget*>(ui->downloadList->itemWidget(item));
            if (piw->getTaskId() == _piw->getTaskId())
            {
                ui->downloadList->takeItem(row);
                break;
            }
        }
        addDownloadTask(filepath, filesize, WorkType::CONTINUE_WORK);
    });
    connect(dw, &DownloadWorker::workFinsh, [this](int status, int taskId, QString msg){
        downloadWorkFinshHandler(status, taskId, msg);
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

UploadWorker::UploadWorker(int _tid, QString _filepath, QString _diskPath, WorkType _wt)
    : filepath(_filepath),
    diskpath(_diskPath),
    isCancel(false),
    isPause(false),
    tid(_tid),
    wt(_wt)
{
    filename = filepath.mid(filepath.lastIndexOf('/') + 1);
}

UploadWorker::~UploadWorker()
{
//    qDebug() << "uploadworker delete";
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
    QMetaObject::Connection conn1, conn2, conn3;
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
        });
        conn3 = connect(this, &UploadWorker::pause, [this](){
            isPause = true;
        });

        if (WorkType::NEW_WORK == wt) // 发送start指令
        {
            MsgUnit* munit = MsgTools::generateUploadFileStartRequest(filename, diskpath);
            socket->write((char*)munit, munit->totalLen);
            if (!socket->waitForBytesWritten())
            {
                emit workFinsh(-1, tid, socket->errorString());
                free(munit);
                break;
            }
            free(munit);
        }
        else // 发送continue指令
        {
            MsgUnit* munit = MsgTools::generateUploadFileContinueRequest(filename, diskpath);
            socket->write((char*)munit, munit->totalLen);
            if (!socket->waitForBytesWritten())
            {
                emit workFinsh(-1, tid, socket->errorString());
                free(munit);
                break;
            }
            free(munit);
        }

        QEventLoop loop;
        connect(this, &UploadWorker::workFinsh, [&loop](){
            loop.quit();
        });
        loop.exec();
    } while (0);

    disconnect(conn1);
    disconnect(conn2);
    disconnect(conn3);
    if (nullptr != socket && socket->state() == QAbstractSocket::ConnectedState)
    {
        socket->close();
        delete socket;
        socket = nullptr;
    }

    deleteLater();
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
    else if (isPause)
    {
        // 发送暂停任务请求
        MsgUnit* req = MsgTools::generateUploadFilePauseRequest();
        socket->write((char*)req, req->totalLen);
        if (!socket->waitForBytesWritten(5000))
        {
            qDebug() << "请重试暂停";
        }
        free(req);
        isPause = false;
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
        else if ("failure" == recvList[0])
        {
            emit workFinsh(-1, tid, getStatusCodeString(recvList[1].mid(7)));
        }
        else
        {
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

    else if (munit->msgType == MsgType::MSG_TYPE_UPLOADFILE_PAUSE_RESPOND)
    {
        emit workFinsh(1, tid);
    }

    else if (munit->msgType == MsgType::MSG_TYPE_UPLOADFILE_CONTINUE_RESPOND)
    {
        // 检查信息是否有误
        if (3 != recvList.size())
        {
            emit workFinsh(-1, tid, "数据传输错误");
        }
        else if ("failure" == recvList[0])
        {
            emit workFinsh(-1, tid, getStatusCodeString(recvList[1].mid(7)));
        }

        // 发送数据
        else
        {
            qint64 alreadyUploadSize = recvList[1].toLongLong(); // 获取服务器读取的字节数
            emit updateProgress(alreadyUploadSize);
            uploadSize = alreadyUploadSize;

            // 移动文件指针
            if (!file.seek(alreadyUploadSize))
            {
                emit workFinsh(-1, tid, file.errorString());
                free(munit);
                return;
            }

            // 继续上传
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
    }

    free(munit);
}

DownloadWorker::DownloadWorker(int _tid, QString _downloadPath, qint64 _fileSize, QString _storePath, WorkType _wt) :
    tid(_tid),
    downloadPath(_downloadPath),
    fileSize(_fileSize),
    isCancel(false),
    isPause(false),
    storePath(_storePath),
    wt(_wt)
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

    else if (isPause)
    {
        // 直接退出线程，关闭连接
        emit workFinsh(1, tid);
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
        {
            fileSize = recvList[1].toLong();
            MsgUnit* req = MsgTools::generateDownloadFileDataRequest(downloadSize);
            socket->write((char*)req, req->totalLen);
            if (!socket->waitForBytesWritten(5000))
                emit workFinsh(-1, tid, socket->errorString());
            free(req);
        }
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

    else if (munit->msgType == MsgType::MSG_TYPE_DOWNLOADFILE_CONTINUE_RESPOND)
    {
        qDebug() << "get continue";
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

    free(munit);
}

void DownloadWorker::run()
{
    QTcpSocket* socket = nullptr;
    QMetaObject::Connection conn1, conn2, conn3;
    do
    {
        QString storeFilePath = storePath + "/" + filename;
        // 打开文件
        if (WorkType::NEW_WORK == wt)
        {
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
        }
        else
        {
            if (!QFile::exists(storeFilePath))
            {
                emit workFinsh(-1, tid, "文件已损坏");
                break;
            }
            file.setFileName(storeFilePath);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Append))
            {
                emit workFinsh(-1, tid, "文件打开失败");
                break;
            }
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
        conn3 = connect(this, &DownloadWorker::pause, [this]{
            isPause = true;
        });

        if (wt == WorkType::NEW_WORK) // 发送start指令，并等待响应
        {
            MsgUnit* munit = MsgTools::generateDownloadFileStartRequest(downloadPath);
            socket->write((char*)munit, munit->totalLen);
            if (!socket->waitForBytesWritten())
            {
                emit workFinsh(-1, tid, socket->errorString());
                break;
            }
        }
        else // 发送continue指令
        {
            downloadSize = file.size();
            MsgUnit* munit = MsgTools::generateDownloadFileContinueRequest(downloadPath, downloadSize);
            socket->write((char*)munit, munit->totalLen);
            if (!socket->waitForBytesWritten())
            {
                emit workFinsh(-1, tid, socket->errorString());
                break;
            }
            emit updateProgress(downloadSize);
        }

        QEventLoop loop;
        connect(this, &DownloadWorker::workFinsh, [&loop](){
            loop.quit();
        });
        loop.exec();
    } while (0);

    qDebug() << "loop exit";

    disconnect(conn1);
    disconnect(conn2);
    if (nullptr != socket && socket->state() == QAbstractSocket::ConnectedState)
    {
        socket->close();
        delete socket;
        socket = nullptr;
    }

    deleteLater();
}


