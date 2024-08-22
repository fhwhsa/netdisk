#include "settingdialog.h"
#include "ui_settingdialog.h"

#include <QFileDialog>
#include <QString>
#include <QSettings>

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
    QSettings ini = QSettings("./setting.ini", QSettings::IniFormat);
    ini.beginGroup("DOWNLOADSETTING");
    QString path = ini.value("PATH").toString();
    ini.endGroup();
    ui->downloadLocation->setText(path);
}

void SettingDialog::iniSignalSlots()
{
    connect(ui->selectDownloadLocBtn, &QPushButton::clicked, this, &SettingDialog::clickSelectDownloadLocBtn);
}

void SettingDialog::clickSelectDownloadLocBtn()
{
    QString downloadLoc = QFileDialog::getExistingDirectory(this);
    if ("" == downloadLoc)
        return;

    ui->downloadLocation->setText(downloadLoc);
    QSettings ini = QSettings("./setting.ini", QSettings::IniFormat);
    ini.beginGroup("DOWNLOADSETTING");
    ini.setValue("PATH", downloadLoc);
    ini.endGroup();

}
