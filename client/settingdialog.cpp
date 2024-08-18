#include "settingdialog.h"
#include "ui_settingdialog.h"

#include <QFileDialog>
#include <QString>

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);

    init();
    iniSignalSlots();
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::init()
{
    ui->downloadLocation->setReadOnly(true);
}

void SettingDialog::iniSignalSlots()
{
    connect(ui->selectDownloadLocBtn, &QPushButton::clicked, this, &SettingDialog::clickSelectDownloadLocBtn);
}

void SettingDialog::clickSelectDownloadLocBtn()
{
    QString downloadLoc = QFileDialog::getOpenFileName(this);
    if ("" == downloadLoc)
        return;

    ui->downloadLocation->setText(downloadLoc);
}
