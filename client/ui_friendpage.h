/********************************************************************************
** Form generated from reading UI file 'friendpage.ui'
**
** Created by: Qt User Interface Compiler version 6.2.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRIENDPAGE_H
#define UI_FRIENDPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FriendPage
{
public:
    QLabel *label;

    void setupUi(QWidget *FriendPage)
    {
        if (FriendPage->objectName().isEmpty())
            FriendPage->setObjectName(QString::fromUtf8("FriendPage"));
        FriendPage->resize(613, 399);
        label = new QLabel(FriendPage);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(180, 220, 171, 17));

        retranslateUi(FriendPage);

        QMetaObject::connectSlotsByName(FriendPage);
    } // setupUi

    void retranslateUi(QWidget *FriendPage)
    {
        FriendPage->setWindowTitle(QCoreApplication::translate("FriendPage", "Form", nullptr));
        label->setText(QCoreApplication::translate("FriendPage", "\345\245\275\345\217\213\345\212\237\350\203\275\351\241\265", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FriendPage: public Ui_FriendPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRIENDPAGE_H
