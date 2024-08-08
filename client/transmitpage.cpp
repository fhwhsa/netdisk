#include "transmitpage.h"
#include "ui_transmitpage.h"

#include "progressitemwidget.h"

#include <QFile>
#include <QString>
#include <QListWidgetItem>
#include <QSize>

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

bool TransmitPage::addUploadTask(QString filepath)
{
    // 达到最大上传任务数，禁止继续添加上传任务
    if (ui->uploadList->count() >= maxUploadTaskNum)
        return false;

    QString filename = filepath.mid(filepath.lastIndexOf('/') + 1);
    QFile* file = new QFile(filepath);
    if (!file->open(QIODevice::ReadOnly))
        return false;

    ProgressItemWidget* piw = new ProgressItemWidget(file, filename, ui->uploadList);
    QListWidgetItem* item = new QListWidgetItem(ui->uploadList);
    ui->uploadList->setItemWidget(item, piw);
    item->setSizeHint(QSize(ui->uploadList->width() - 10, piw->height()));
    return true;
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
