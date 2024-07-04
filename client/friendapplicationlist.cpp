#include "friendapplicationlist.h"
#include "ui_friendapplicationlist.h"
#include "bubbletips.h"
#include "msgtools.h"
#include "friendpage.h"
#include "alistitemwidget.h"
#include "respondwatcher.h"

#include <QMessageBox>
#include <QDebug>

FriendApplicationList::FriendApplicationList(FriendPage* _parent) :
    parent(_parent),
    ui(new Ui::FriendApplicationList)
{
    ui->setupUi(this);

    init();
    iniSignalSlots();    
}

FriendApplicationList::~FriendApplicationList()
{
    delete ui;
}

void FriendApplicationList::refreshManually()
{
    RespondWatcher::create(parent, SIGNAL(respondGetFriendApplication(std::shared_ptr<MsgUnit>)), "等待响应超时，请检查网络", 2, centerPos,
        [this](std::shared_ptr<MsgUnit> sptr){
            flushList(sptr);
        });
    emit getApplicaionList();
}

void FriendApplicationList::init()
{
    this->setWindowTitle(" ");
    ui->applicationList->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->groupBox->setStyleSheet("QGroupBox {"
                                "border: 2px solid black;"
                                "border-radius: 10px;"
                                "margin-top: 20px;"
                                "}");

    centerPos = this->pos() + QPoint(this->width() / 2, this->height() / 2);
}

void FriendApplicationList::iniSignalSlots()
{
    connect(ui->applicationList, &QListWidget::itemDoubleClicked, this, &FriendApplicationList::itemDoubleClicked);
//    connect(parent, &FriendPage::respondGetFriendApplication, this, &FriendApplicationList::flushList);
}

void FriendApplicationList::addItem(QString lt, QString lb, QString r, QString id, QString flag)
{
    QListWidgetItem* item = new QListWidgetItem(ui->applicationList);
    AListItemWidget* iw = new AListItemWidget(lt, lb, r, id, flag, ui->applicationList);
    ui->applicationList->setItemWidget(item, iw);
    item->setSizeHint(iw->sizeHint());
}

void FriendApplicationList::itemDoubleClicked(QListWidgetItem *item)
{
    AListItemWidget* iw = (AListItemWidget*)ui->applicationList->itemWidget(item);
    if ("3" == iw->getFlag())
    {
        QMessageBox::StandardButton op = QMessageBox::question(this, " ", "同意请求？", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);;
        if (QMessageBox::Yes == op)
        {
            qDebug() << "yes";
            emit verifyFriend(iw->getId(), true);
        }
        else if (QMessageBox::No == op)
        {
            qDebug() << "no";
            emit verifyFriend(iw->getId(), false);
            emit getApplicaionList();
        }
        else
            qDebug() << "Cancel";
    }
}

void FriendApplicationList::flushList(std::shared_ptr<MsgUnit> sptr)
{
    for (int i = ui->applicationList->count() - 1; ~i; --i)
    {
        QListWidgetItem* item = ui->applicationList->item(i);
        AListItemWidget* iw = (AListItemWidget*)ui->applicationList->itemWidget(item);
        if (nullptr != iw)
        {
            iw->deleteLater();
            ui->applicationList->setItemWidget(item, nullptr);
        }
        ui->applicationList->takeItem(i);
    }

    QStringList msg = MsgTools::getAllRows(sptr.get());
    for (const QString& str : msg)
    {
        QStringList list = str.split('|');
        if (4 != list.size())
            continue;

        QString lb, r;
        QString flag = list[2];
        if (flag <= "2")
            lb = "请求添加对方为好友";
        else
            lb = "对方请求加为好友";

        if ("1" == flag || "3" == flag)
            r = "等待验证";
        else
            r = "已拒绝";

        addItem(list[1], lb, r, list[0], flag);
    }
}
