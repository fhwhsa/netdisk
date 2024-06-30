#include "friendapplicationlist.h"
#include "ui_friendapplicationlist.h"
#include "bubbletips.h"

#include <QMessageBox>
#include <QDebug>

FriendApplicationList::FriendApplicationList(QStringList databaseRecord, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FriendApplicationList)
{
    ui->setupUi(this);

    init(databaseRecord);
    iniSignalSlots();
}

FriendApplicationList::~FriendApplicationList()
{
    delete ui;
}

void FriendApplicationList::init(QStringList _databaseRecord)
{
    this->setWindowTitle(" ");
    ui->applicationList->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->groupBox->setStyleSheet("QGroupBox {"
                                "border: 2px solid black;"
                                "border-radius: 10px;"
                                "margin-top: 20px;"
                                "}");
    for (const QString& str : _databaseRecord)
    {
        if ("" == str)
            continue;
        QString left = str.first(str.lastIndexOf(':'));
        QString flag = str.mid(str.lastIndexOf(':') + 1);
        QString right;
        if ("0" == flag)
            right = "请求添加对方为好友，等待验证";
        else if ("1" == flag)
            right = "请求添加对方为好友，已同意";
        else if ("2" == flag)
            right = "请求添加对方为好友，已拒绝";
        else if ("3" == flag)
            right = "对方请求加为好友，等待验证";
        else if ("4" == flag)
            right = "对方请求加为好友，已同意";
        else if ("5" == flag)
            right = "对方请求加为好友，已拒绝";
        else
            right = "error";

        addItem(left, right, left.first(left.indexOf(':')), flag);
    }
}

void FriendApplicationList::iniSignalSlots()
{
    connect(ui->applicationList, &QListWidget::itemDoubleClicked, this, &FriendApplicationList::itemDoubleClicked);
}

void FriendApplicationList::addItem(QString left, QString right, QString id, QString flag)
{
    QListWidgetItem* item = new QListWidgetItem(ui->applicationList);
    CustomWidget* cw = new CustomWidget(left, right, id, flag);
    ui->applicationList->setItemWidget(item, cw);
    item->setSizeHint(cw->sizeHint());
}

void FriendApplicationList::itemDoubleClicked(QListWidgetItem *item)
{
    CustomWidget* cw = (CustomWidget*)ui->applicationList->itemWidget(item);
    if ("3" == cw->getFlag())
    {
        if (QMessageBox::Yes ==  QMessageBox::question(this, " ", "同意请求？", QMessageBox::Yes | QMessageBox::No))
            qDebug() << "yes";
        else
            qDebug() << "no";
    }
    else
        BubbleTips::showBubbleTips(cw->getText(), 1, this);
}
