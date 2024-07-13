#include "folderpage.h"
#include "ui_folderpage.h"

#include "msgtools.h"
#include "bubbletips.h"
#include "respondwatcher.h"

#include <QDebug>
#include <QToolButton>
#include <QIcon>

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

    if (list.size() >= 2 && list[0] == "failure")
    {
        BubbleTips::showBubbleTips(list[1], 2, this);
        return;
    }

    for (int i = ui->fileList->count() - 1; ~i; --i)
        ui->fileList->takeItem(i);

    for (const QString& str : list)
    {
        int pos = str.lastIndexOf('|');
        if (-1 == pos)
            continue;
        QListWidgetItem* item = new QListWidgetItem(ui->fileList);
        if ("0" == str.mid(pos + 1))
        {
            item->setStatusTip("0");
            item->setIcon(QIcon(":/img/res/img/folder.png"));
        }
        else
        {
            item->setStatusTip("1");
            item->setIcon(QIcon(":/img/res/img/file.png"));
        }

        item->setText(str.first(pos));
        item->setSizeHint(itemSize);
    }
}

void FolderPage::clickTbAddFolder()
{
    qDebug() << "add folder";
}

void FolderPage::clickTbDelete()
{
    qDebug() << "delete";
}

void FolderPage::clickTbRename()
{
    qDebug() << "rename";
}

void FolderPage::clickTbDownload()
{
    qDebug() << "download";
}

void FolderPage::clickTbUpload()
{
    qDebug() << "upload";
}

void FolderPage::clickTbFlush()
{
    RespondWatcher::create(this, SIGNAL(getFolderContentRespond(std::shared_ptr<MsgUnit>)), "文件列表刷新超时", 3,
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
    RespondWatcher::create(this, SIGNAL(getFolderContentRespond(std::shared_ptr<MsgUnit>)), "文件列表刷新超时", 3,
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
    RespondWatcher::create(this, SIGNAL(getFolderContentRespond(std::shared_ptr<MsgUnit>)), "文件列表刷新超时", 3,
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
}
