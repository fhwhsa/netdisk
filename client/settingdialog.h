#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class SettingDialog;
}

/**
 * @brief 设置对话框
 */
class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = nullptr);
    ~SettingDialog();

private:
    Ui::SettingDialog *ui;

    void init();
    void iniSignalSlots();

private slots:
    void clickSelectDownloadLocBtn();
};

#endif // SETTINGDIALOG_H
