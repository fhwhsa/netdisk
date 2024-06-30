#include "friendapplicationlist.h"
#include "ui_friendapplicationlist.h"

#include <QListWidgetItem>

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
        if ("-1" == flag)
            right = "对方已拒绝您的申请";
        else if ("0" == flag)
            right = "等待对方通过验证";
        else if ("1" == flag)
            right = "对方已同意好友申请";
        else if ("2" == flag)
            right = "对方请求添加您为好友";
        else
            right = "error";

        addItem(left, right);
    }
}

void FriendApplicationList::iniSignalSlots()
{

}

void FriendApplicationList::addItem(QString left, QString right)
{
    QListWidgetItem* item = new QListWidgetItem(ui->applicationList);
    CustomWidget* cw = new CustomWidget(left, right);
    ui->applicationList->setItemWidget(item, cw);
    item->setSizeHint(cw->sizeHint());
}
