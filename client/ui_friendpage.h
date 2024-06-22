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
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FriendPage
{
public:
    QHBoxLayout *horizontalLayout_4;
    QHBoxLayout *horizontalLayout_3;
    QGroupBox *friendsList;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QToolButton *tb_newFriend;
    QHBoxLayout *horizontalLayout;
    QLineEdit *searchLine;
    QToolButton *tb_search;
    QListWidget *listWidget;
    QGroupBox *chatBox;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_3;
    QLabel *label;
    QPlainTextEdit *plainTextEdit;
    QGroupBox *groupBox_2;
    QLabel *label_2;
    QPlainTextEdit *plainTextEdit_2;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QToolButton *tb_clear;
    QToolButton *tb_send;
    QGroupBox *welcomeBox;
    QLabel *label_3;

    void setupUi(QWidget *FriendPage)
    {
        if (FriendPage->objectName().isEmpty())
            FriendPage->setObjectName(QString::fromUtf8("FriendPage"));
        FriendPage->resize(1023, 597);
        horizontalLayout_4 = new QHBoxLayout(FriendPage);
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        friendsList = new QGroupBox(FriendPage);
        friendsList->setObjectName(QString::fromUtf8("friendsList"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(friendsList->sizePolicy().hasHeightForWidth());
        friendsList->setSizePolicy(sizePolicy);
        friendsList->setMinimumSize(QSize(280, 0));
        friendsList->setMaximumSize(QSize(280, 16777215));
        friendsList->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        friendsList->setFlat(false);
        verticalLayout_2 = new QVBoxLayout(friendsList);
        verticalLayout_2->setSpacing(2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(2, 2, 2, 2);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tb_newFriend = new QToolButton(friendsList);
        tb_newFriend->setObjectName(QString::fromUtf8("tb_newFriend"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tb_newFriend->sizePolicy().hasHeightForWidth());
        tb_newFriend->setSizePolicy(sizePolicy1);

        verticalLayout->addWidget(tb_newFriend);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        searchLine = new QLineEdit(friendsList);
        searchLine->setObjectName(QString::fromUtf8("searchLine"));

        horizontalLayout->addWidget(searchLine);

        tb_search = new QToolButton(friendsList);
        tb_search->setObjectName(QString::fromUtf8("tb_search"));

        horizontalLayout->addWidget(tb_search);


        verticalLayout->addLayout(horizontalLayout);

        listWidget = new QListWidget(friendsList);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        verticalLayout->addWidget(listWidget);


        verticalLayout_2->addLayout(verticalLayout);


        horizontalLayout_3->addWidget(friendsList);

        chatBox = new QGroupBox(FriendPage);
        chatBox->setObjectName(QString::fromUtf8("chatBox"));
        verticalLayout_4 = new QVBoxLayout(chatBox);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label = new QLabel(chatBox);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_3->addWidget(label);

        plainTextEdit = new QPlainTextEdit(chatBox);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setReadOnly(true);

        verticalLayout_3->addWidget(plainTextEdit);

        groupBox_2 = new QGroupBox(chatBox);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 10, 67, 17));

        verticalLayout_3->addWidget(groupBox_2);

        plainTextEdit_2 = new QPlainTextEdit(chatBox);
        plainTextEdit_2->setObjectName(QString::fromUtf8("plainTextEdit_2"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(plainTextEdit_2->sizePolicy().hasHeightForWidth());
        plainTextEdit_2->setSizePolicy(sizePolicy2);
        plainTextEdit_2->setMinimumSize(QSize(0, 120));
        plainTextEdit_2->setMaximumSize(QSize(16777215, 120));

        verticalLayout_3->addWidget(plainTextEdit_2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        tb_clear = new QToolButton(chatBox);
        tb_clear->setObjectName(QString::fromUtf8("tb_clear"));

        horizontalLayout_2->addWidget(tb_clear);

        tb_send = new QToolButton(chatBox);
        tb_send->setObjectName(QString::fromUtf8("tb_send"));

        horizontalLayout_2->addWidget(tb_send);


        verticalLayout_3->addLayout(horizontalLayout_2);


        verticalLayout_4->addLayout(verticalLayout_3);


        horizontalLayout_3->addWidget(chatBox);

        welcomeBox = new QGroupBox(FriendPage);
        welcomeBox->setObjectName(QString::fromUtf8("welcomeBox"));
        label_3 = new QLabel(welcomeBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(30, 30, 67, 17));

        horizontalLayout_3->addWidget(welcomeBox);


        horizontalLayout_4->addLayout(horizontalLayout_3);


        retranslateUi(FriendPage);

        QMetaObject::connectSlotsByName(FriendPage);
    } // setupUi

    void retranslateUi(QWidget *FriendPage)
    {
        FriendPage->setWindowTitle(QCoreApplication::translate("FriendPage", "Form", nullptr));
        friendsList->setTitle(QString());
        tb_newFriend->setText(QCoreApplication::translate("FriendPage", "\346\226\260\345\245\275\345\217\213", nullptr));
        tb_search->setText(QCoreApplication::translate("FriendPage", "...", nullptr));
        chatBox->setTitle(QString());
        label->setText(QCoreApplication::translate("FriendPage", "TextLabel", nullptr));
        groupBox_2->setTitle(QString());
        label_2->setText(QCoreApplication::translate("FriendPage", "\345\267\245\345\205\267\346\240\217", nullptr));
        tb_clear->setText(QCoreApplication::translate("FriendPage", "...", nullptr));
        tb_send->setText(QCoreApplication::translate("FriendPage", "...", nullptr));
        welcomeBox->setTitle(QString());
        label_3->setText(QCoreApplication::translate("FriendPage", "\344\275\240\345\245\275", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FriendPage: public Ui_FriendPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRIENDPAGE_H
