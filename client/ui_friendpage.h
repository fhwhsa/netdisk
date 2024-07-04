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
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
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
    QGroupBox *friendBox;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_5;
    QToolButton *tb_notification;
    QFrame *line;
    QToolButton *tb_newFriend;
    QHBoxLayout *horizontalLayout;
    QLineEdit *searchLine;
    QToolButton *tb_search;
    QToolButton *tb_flushFriendList;
    QListWidget *friendList;
    QGroupBox *chatBox;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_3;
    QLabel *sessionName;
    QPlainTextEdit *chatContent;
    QGroupBox *groupBox_2;
    QLabel *label_2;
    QPlainTextEdit *inputBox;
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
        FriendPage->resize(934, 597);
        horizontalLayout_4 = new QHBoxLayout(FriendPage);
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        friendBox = new QGroupBox(FriendPage);
        friendBox->setObjectName(QString::fromUtf8("friendBox"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(friendBox->sizePolicy().hasHeightForWidth());
        friendBox->setSizePolicy(sizePolicy);
        friendBox->setMinimumSize(QSize(300, 0));
        friendBox->setMaximumSize(QSize(300, 16777215));
        friendBox->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        friendBox->setFlat(false);
        verticalLayout_2 = new QVBoxLayout(friendBox);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        tb_notification = new QToolButton(friendBox);
        tb_notification->setObjectName(QString::fromUtf8("tb_notification"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tb_notification->sizePolicy().hasHeightForWidth());
        tb_notification->setSizePolicy(sizePolicy1);
        tb_notification->setMinimumSize(QSize(27, 27));
        tb_notification->setCursor(QCursor(Qt::PointingHandCursor));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/img/res/img/notification.png"), QSize(), QIcon::Normal, QIcon::Off);
        tb_notification->setIcon(icon);

        horizontalLayout_5->addWidget(tb_notification);

        line = new QFrame(friendBox);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout_5->addWidget(line);

        tb_newFriend = new QToolButton(friendBox);
        tb_newFriend->setObjectName(QString::fromUtf8("tb_newFriend"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(tb_newFriend->sizePolicy().hasHeightForWidth());
        tb_newFriend->setSizePolicy(sizePolicy2);
        tb_newFriend->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout_5->addWidget(tb_newFriend);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        searchLine = new QLineEdit(friendBox);
        searchLine->setObjectName(QString::fromUtf8("searchLine"));

        horizontalLayout->addWidget(searchLine);

        tb_search = new QToolButton(friendBox);
        tb_search->setObjectName(QString::fromUtf8("tb_search"));
        tb_search->setCursor(QCursor(Qt::PointingHandCursor));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/img/res/img/search.png"), QSize(), QIcon::Normal, QIcon::Off);
        tb_search->setIcon(icon1);

        horizontalLayout->addWidget(tb_search);

        tb_flushFriendList = new QToolButton(friendBox);
        tb_flushFriendList->setObjectName(QString::fromUtf8("tb_flushFriendList"));
        tb_flushFriendList->setCursor(QCursor(Qt::PointingHandCursor));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/img/res/img/flush.png"), QSize(), QIcon::Normal, QIcon::Off);
        tb_flushFriendList->setIcon(icon2);

        horizontalLayout->addWidget(tb_flushFriendList);


        verticalLayout->addLayout(horizontalLayout);

        friendList = new QListWidget(friendBox);
        friendList->setObjectName(QString::fromUtf8("friendList"));

        verticalLayout->addWidget(friendList);


        verticalLayout_2->addLayout(verticalLayout);


        horizontalLayout_3->addWidget(friendBox);

        chatBox = new QGroupBox(FriendPage);
        chatBox->setObjectName(QString::fromUtf8("chatBox"));
        verticalLayout_4 = new QVBoxLayout(chatBox);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        sessionName = new QLabel(chatBox);
        sessionName->setObjectName(QString::fromUtf8("sessionName"));

        verticalLayout_3->addWidget(sessionName);

        chatContent = new QPlainTextEdit(chatBox);
        chatContent->setObjectName(QString::fromUtf8("chatContent"));
        chatContent->setReadOnly(true);

        verticalLayout_3->addWidget(chatContent);

        groupBox_2 = new QGroupBox(chatBox);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 10, 67, 17));

        verticalLayout_3->addWidget(groupBox_2);

        inputBox = new QPlainTextEdit(chatBox);
        inputBox->setObjectName(QString::fromUtf8("inputBox"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(inputBox->sizePolicy().hasHeightForWidth());
        inputBox->setSizePolicy(sizePolicy3);
        inputBox->setMinimumSize(QSize(0, 120));
        inputBox->setMaximumSize(QSize(16777215, 120));

        verticalLayout_3->addWidget(inputBox);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        tb_clear = new QToolButton(chatBox);
        tb_clear->setObjectName(QString::fromUtf8("tb_clear"));
        tb_clear->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout_2->addWidget(tb_clear);

        tb_send = new QToolButton(chatBox);
        tb_send->setObjectName(QString::fromUtf8("tb_send"));
        tb_send->setCursor(QCursor(Qt::PointingHandCursor));

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
        friendBox->setTitle(QString());
        tb_notification->setText(QCoreApplication::translate("FriendPage", "...", nullptr));
        tb_newFriend->setText(QCoreApplication::translate("FriendPage", "\346\226\260\345\245\275\345\217\213", nullptr));
        tb_search->setText(QCoreApplication::translate("FriendPage", "...", nullptr));
        tb_flushFriendList->setText(QCoreApplication::translate("FriendPage", "...", nullptr));
        chatBox->setTitle(QString());
        sessionName->setText(QCoreApplication::translate("FriendPage", "TextLabel", nullptr));
        groupBox_2->setTitle(QString());
        label_2->setText(QCoreApplication::translate("FriendPage", "\345\267\245\345\205\267\346\240\217", nullptr));
        tb_clear->setText(QCoreApplication::translate("FriendPage", "\346\270\205\347\251\272", nullptr));
        tb_send->setText(QCoreApplication::translate("FriendPage", "\345\217\221\351\200\201", nullptr));
        welcomeBox->setTitle(QString());
        label_3->setText(QCoreApplication::translate("FriendPage", "\344\275\240\345\245\275", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FriendPage: public Ui_FriendPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRIENDPAGE_H
