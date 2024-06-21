#include "folderpage.h"
#include "ui_folderpage.h"

#include <QString>

FolderPage::FolderPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FolderPage)
{
    ui->setupUi(this);
}

FolderPage::~FolderPage()
{
    delete ui;
}
