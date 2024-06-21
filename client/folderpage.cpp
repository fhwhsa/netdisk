#include "folderpage.h"
#include "ui_folderpage.h"

#include <QString>
#include <QDebug>

FolderPage::FolderPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FolderPage)
{
    ui->setupUi(this);
}

FolderPage::~FolderPage()
{
//    qDebug() << "folderpage delete";
    delete ui;
}
