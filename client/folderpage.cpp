#include "folderpage.h"
#include "ui_folderpage.h"

#include "msgtools.h"
#include "bubbletips.h"
#include "respondwatcher.h"
#include "statusCode.h"

#include <QDebug>
#include <QToolButton>
#include <QIcon>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QStringList>

QSize FolderPage::itemSize = QSize(0, 50);

FolderPage::FolderPage(QString _userId, QString _userEmail,QWidget *parent) :
    userId(_userId),
    userEmail(_userEmail),
    QWidget(parent),
    ui(new Ui::FolderPage)
{
    ui->setupUi(this);

    init();
    iniSignalSlots();
}

FolderPage::~FolderPage()
{
    delete ui;
}

void FolderPage::refreshFileListManually()
{
    clickTbFlush();
}

void FolderPage::init()
{
    currPath = QString("/" + userId);
    ui->tb_back->setEnabled(false);
    ui->fileList->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

void FolderPage::iniSignalSlots()
{
    connect(ui->tb_addfolder, &QToolButton::clicked, this, &FolderPage::clickTbAddFolder);
    connect(ui->tb_delete, &QToolButton::clicked, this, &FolderPage::clickTbDelete);
    connect(ui->tb_rename, &QToolButton::clicked, this, &FolderPage::clickTbRename);
    connect(ui->tb_upload, &QToolButton::clicked, this, &FolderPage::clickTbUpload);
    connect(ui->tb_download, &QToolButton::clicked, this, &FolderPage::clickTbDownload);
    connect(ui->tb_share, &QToolButton::clicked, this, &FolderPage::clickTbShare);
    connect(ui->tb_flush, &QToolButton::clicked, this, &FolderPage::clickTbFlush);
    connect(ui->tb_back, &QToolButton::clicked, this, &FolderPage::clickTbBack);

    connect(ui->fileList, &QListWidget::itemDoubleClicked, this, &FolderPage::itemDoubleClick);
}

void FolderPage::flushFileList(std::shared_ptr<MsgUnit> sptr)
{
    QStringList list = MsgTools::getAllRows(sptr.get());

    if (list.size() > 2 && list[0] == "failure")
    {
        QString statusCode = list[1];
        if (statusCode.length() <= 7)
            BubbleTips::showBubbleTips("通信错误", 2, this);
        else
            BubbleTips::showBubbleTips(getStatusCodeString(statusCode.mid(7)), 2, this);
        return;
    }

    for (int i = ui->fileList->count() - 1; ~i; --i)
        ui->fileList->takeItem(i);

    for (const QString& str : list)
    {
        if ("" == str)
            continue;
        QStringList info = str.split('|');
        if (info.size() != 3)
        {
            BubbleTips::showBubbleTips("目录文件信息出错", 2, this);
            continue;
        }
        QListWidgetItem* item = new QListWidgetItem(ui->fileList);
        if (info[1] == "0")
        {
            item->setStatusTip("0");
            item->setIcon(QIcon(":/img/res/img/folder.png"));
            item->setText(info[0]);
        }
        else
        {
            item->setStatusTip("1");
            item->setIcon(QIcon(":/img/res/img/file.png"));
            item->setText(info[0] + ":" + info[2]);
        }
        item->setSizeHint(itemSize);
    }
}

void FolderPage::clickTbAddFolder()
{
    bool ok;
    QString name = QInputDialog::getText(this, "新建文件夹", "输入文件夹名称：", QLineEdit::Normal, "", &ok);
    if (!ok) return;
    if (name.isEmpty())
    {
        BubbleTips::showBubbleTips("文件夹名称不能为空", 1, this);
        return;
    }
    for (int i = ui->fileList->count() - 1; ~i; --i)
    {
        if (name == ui->fileList->item(i)->text())
        {
            BubbleTips::showBubbleTips("无法创建文件夹\"" + name + "\"：文件已存在", 1, this);
            return;
        }
    }

    // 发送请求
    RespondWatcher::createBgRw(this, SIGNAL(getCreateFolderRespond(std::shared_ptr<MsgUnit>)), "创建文件夹响应超时", 3,
            QPoint(this->pos().rx() + this->width() / 2, this->pos().ry() + this->height() / 2),
                           [this](std::shared_ptr<MsgUnit> sptr){
                                QStringList list = MsgTools::getAllRows(sptr.get());
                                if (list.size() > 2 && list[0] == "failure")
                                {
                                    QString statusCode = list[1];
                                    if (statusCode.length() <= 7)
                                        BubbleTips::showBubbleTips("通信错误", 2, this);
                                    else
                                        BubbleTips::showBubbleTips(getStatusCodeString(statusCode.mid(7)), 2, this);
                                }
                                else
                                    flushFileList(sptr);
                           });
    emit _sendMsg(MsgTools::generateCreateFolderRequest(currPath, name));
}

void FolderPage::clickTbDelete()
{
    QList<QListWidgetItem*> selectedList = ui->fileList->selectedItems();
    if (selectedList.isEmpty())
    {
        BubbleTips::showBubbleTips("请选择要删除的文件/文件夹", 1, this);
        return;
    }

    QString str;
    for (const QListWidgetItem* item : selectedList)
    {
        str.append(item->text() + "\n");
    }
    if (QMessageBox::No == QMessageBox::question(this, "确定删除以下文件/文件夹吗？", str, QMessageBox::Yes | QMessageBox::No))
        return;

    QList<QString> paths = str.split('\n');
    for (QString& it : paths)
        it = currPath + "/" + it;
    paths.pop_back(); // 去除空行

    // 发送请求
    RespondWatcher::createBgRw(this, SIGNAL(getDeleteFileOrFolderRespond(std::shared_ptr<MsgUnit>)), "删除文件夹响应超时", 3,
            QPoint(this->pos().rx() + this->width() / 2, this->pos().ry() + this->height() / 2),
                           [this](std::shared_ptr<MsgUnit> sptr){
                                QStringList content = MsgTools::getAllRows(sptr.get());
                                if (content.size() > 0 && content[0] != "success")
                                {
                                    QString tips = "以下文件删除失败：\n";
                                    for (int i = 1; i < content.size(); ++i)
                                        tips.append(content[i] + "\n");
                                    QMessageBox::information(this, " ", tips);
                                }
                                this->clickTbFlush();
                           });
    emit _sendMsg(MsgTools::generateDeleteFileOrFolderRequest(paths));
}

void FolderPage::clickTbRename()
{
    if (ui->fileList->selectedItems().size() > 1)
    {
        BubbleTips::showBubbleTips("不支持重命名多个文件/文件夹", 1, this);
        return;
    }
    if (0 == ui->fileList->selectedItems().size())
    {
        BubbleTips::showBubbleTips("请选择要重命名的文件/文件夹", 1, this);
        return;
    }

    bool ok;
    QString path = currPath + "/" + ui->fileList->currentItem()->text();
    QString newName = QInputDialog::getText(this, "重命名文件/文件夹", "新名称：", QLineEdit::Normal, "", &ok);
    if (!ok) return;
    if (newName.isEmpty())
    {
        BubbleTips::showBubbleTips("文件/文件夹名不能为空", 1, this);
        return;
    }

    for (int i = ui->fileList->count() - 1; ~i; --i)
    {
        if (ui->fileList->item(i)->text() == newName)
        {
            BubbleTips::showBubbleTips("同名文件/文件夹已存在", 1, this);
            return;
        }
    }

    // 发送请求
    RespondWatcher::createBgRw(this, SIGNAL(getRenameFileOrFolderRespond(std::shared_ptr<MsgUnit>)), "重命名响应超时", 3,
            QPoint(this->pos().rx() + this->width() / 2, this->pos().ry() + this->height() / 2),
                           [this](std::shared_ptr<MsgUnit> sptr){
                                QStringList content = MsgTools::getAllRows(sptr.get());
                                if (content.size() > 0 && content[0] == "success")
                                    this->clickTbFlush();
                                else if (content.size() > 2 && content[1].length() > 7)
                                    BubbleTips::showBubbleTips(getStatusCodeString(content[1].mid(7)), 1, this);
                                else
                                    BubbleTips::showBubbleTips("通信错误", 1, this);
                           });
    emit _sendMsg(MsgTools::generateRenameFileOrFolderRequest(path, newName));
}

void FolderPage::clickTbDownload()
{
    QList<QListWidgetItem*> selectedList = ui->fileList->selectedItems();
    if (selectedList.isEmpty())
    {
        BubbleTips::showBubbleTips("请选择要删除的文件/文件夹", 1, this);
        return;
    }

    QStringList fileList;
    for (const QListWidgetItem* item : selectedList)
    {
        if ("1" == item->statusTip())   // 文件
            fileList.push_back(item->text());
    }

    QString str;
    for (const QString& it : fileList)
    {
        str.append(it.first(it.lastIndexOf(':')) + "\n");
    }
    if (QMessageBox::No == QMessageBox::question(this, "确定下载以下文件吗？", str, QMessageBox::Yes | QMessageBox::No))
        return;


    // 测试功能，仅下载第一个
    int pos = fileList[0].lastIndexOf(':');
    emit deliverDownloadTask(currPath + "/" + fileList[0].first(pos), fileList[0].mid(pos + 1).toULong());
}

void FolderPage::clickTbUpload()
{
    QString filepath = QFileDialog::getOpenFileName(this);
    if (filepath.isEmpty())
        return;
    emit deliverUploadTask(filepath, currPath);
}

void FolderPage::clickTbFlush()
{
    RespondWatcher::createBgRw(this, SIGNAL(getFolderContentRespond(std::shared_ptr<MsgUnit>)), "文件列表刷新超时", 3,
            QPoint(this->pos().rx() + this->width() / 2, this->pos().ry() + this->height() / 2),
                           [this](std::shared_ptr<MsgUnit> sptr){
                               flushFileList(sptr);
                           });
    emit _sendMsg(MsgTools::generateGetFolderContentRequest(currPath));
}

void FolderPage::clickTbShare()
{
    qDebug() << "share";
}

void FolderPage::clickTbBack()
{
    QString path = backList.back();
    RespondWatcher::createBgRw(this, SIGNAL(getFolderContentRespond(std::shared_ptr<MsgUnit>)), "文件列表刷新超时", 3,
            QPoint(this->pos().rx() + this->width() / 2, this->pos().ry() + this->height() / 2),
                           [this, path](std::shared_ptr<MsgUnit> sptr){
                               flushFileList(sptr);
                               backList.pop_back();
                               if (backList.isEmpty())
                                   ui->tb_back->setEnabled(false);
                               currPath = path;
                           });
    emit _sendMsg(MsgTools::generateGetFolderContentRequest(path));
}

void FolderPage::itemDoubleClick(QListWidgetItem *item)
{
    if ("1" == item->statusTip())
        return;

    QString path = currPath + "/" + item->text();
    RespondWatcher::createBgRw(this, SIGNAL(getFolderContentRespond(std::shared_ptr<MsgUnit>)), "文件列表刷新超时", 3,
            QPoint(this->pos().rx() + this->width() / 2, this->pos().ry() + this->height() / 2),
                           [this, path](std::shared_ptr<MsgUnit> sptr){
                               flushFileList(sptr);
                               backList.push_back(currPath);
                               ui->tb_back->setEnabled(true);
                               currPath = path;
                           });
    emit _sendMsg(MsgTools::generateGetFolderContentRequest(path));
}

void FolderPage::getMsg(std::shared_ptr<MsgUnit> sptr)
{
    if (MsgType::MSG_TYPE_GETFOLDERCONTENT_RESPOND == sptr->msgType)
        emit getFolderContentRespond(sptr);
    else if (MsgType::MSG_TYPE_CREATERFOLDER_RESPOND == sptr->msgType)
        emit getCreateFolderRespond(sptr);
    else if (MsgType::MSG_TYPE_DELETEFILEFOLDER_RESPOND == sptr->msgType)
        emit getDeleteFileOrFolderRespond(sptr);
    else if (MsgType::MSG_TYPE_RENAMEFILEFOLDER_RESPOND == sptr->msgType)
        emit getRenameFileOrFolderRespond(sptr);
}
