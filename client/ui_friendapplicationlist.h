/********************************************************************************
** Form generated from reading UI file 'friendapplicationlist.ui'
**
** Created by: Qt User Interface Compiler version 6.2.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRIENDAPPLICATIONLIST_H
#define UI_FRIENDAPPLICATIONLIST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_FriendApplicationList
{
public:
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QListWidget *applicationList;

    void setupUi(QDialog *FriendApplicationList)
    {
        if (FriendApplicationList->objectName().isEmpty())
            FriendApplicationList->setObjectName(QString::fromUtf8("FriendApplicationList"));
        FriendApplicationList->resize(641, 327);
        horizontalLayout = new QHBoxLayout(FriendApplicationList);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        groupBox = new QGroupBox(FriendApplicationList);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        applicationList = new QListWidget(groupBox);
        applicationList->setObjectName(QString::fromUtf8("applicationList"));

        verticalLayout->addWidget(applicationList);


        horizontalLayout->addWidget(groupBox);


        retranslateUi(FriendApplicationList);

        QMetaObject::connectSlotsByName(FriendApplicationList);
    } // setupUi

    void retranslateUi(QDialog *FriendApplicationList)
    {
        FriendApplicationList->setWindowTitle(QCoreApplication::translate("FriendApplicationList", "Dialog", nullptr));
        groupBox->setTitle(QCoreApplication::translate("FriendApplicationList", "\345\245\275\345\217\213\351\200\232\347\237\245", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FriendApplicationList: public Ui_FriendApplicationList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRIENDAPPLICATIONLIST_H
