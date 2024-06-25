#include "addfrienddialog.h"
#include "ui_addfrienddialog.h"
#include "bubbletips.h"
#include "friendpage.h"
#include "msgtools.h"

#include <QString>
#include <QMessageBox>
#include <QDebug>
#include <QPoint>
#include <memory>

AddFriendDialog::AddFriendDialog(FriendPage *_parent) :
    parent(_parent),
    ui(new Ui::AddFriendDialog)
{
    ui->setupUi(this);

    init();
    iniSignalSlots();
}

AddFriendDialog::~AddFriendDialog()
{
    delete ui;
}

void AddFriendDialog::init()
{
    this->setWindowTitle(QString("新好友"));
    QString style = "background-color: transparent; border: none;";
    ui->lineEdit->setStyleSheet(style);
    ui->tb_search->setStyleSheet(style);
    ui->tb_add->setStyleSheet("background-color: transparent; border: none;");
    ui->inputBox->setStyleSheet("QGroupBox {"
                                "border: 2px solid black;"
                                "border-radius: 10px;"
                                "margin-top: 20px;"
                                "}");
    ui->resultBox->setStyleSheet("QGroupBox {"
                                 "border-radius: 10px;"
                                 "margin-top: 20px;"
                                 "}");
    ui->resultBox->hide();
    ui->errorBox->hide();

    QPoint pPos = parent->pos();
    this->move(pPos.rx() + parent->width() / 2 - this->width() / 2, pPos.ry() + parent->height() / 2 - this->height() / 2);
}

void AddFriendDialog::iniSignalSlots()
{
    connect(ui->tb_search, &QToolButton::clicked, [this](){
        QString text = ui->lineEdit->text();
        if (text.isEmpty())
        {
            BubbleTips *tips = new BubbleTips("请输入搜索内容", 2, this);
            tips->show();
            return;
        }
        emit _searchUser(text);
    });
    connect(parent, &FriendPage::respondSearch, [this](std::shared_ptr<MsgUnit> sptr){
        ui->errorBox->hide();
        ui->resultBox->hide();
        QString msg = (char*)sptr->msg;
        qDebug() << msg;
        if (msg.first(7) == "success")
        {
            QString email = MsgTools::getRow(sptr.get(), 1);
            if ("" == email)
            {
                ui->errInfo->setText("数据传输损坏");
                ui->errorBox->show();
            }
            else
            {
                ui->targetEmail->setText(email.mid(6));
                ui->resultBox->show();
            }
        }
        else
        {
            QString info = MsgTools::getRow(sptr.get(), 1);
            if ("" == info)
                info = "数据传输损坏";
            ui->errInfo->setText(info);
            ui->errorBox->show();
        }
    });
}
