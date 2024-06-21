/********************************************************************************
** Form generated from reading UI file 'mainpage.ui'
**
** Created by: Qt User Interface Compiler version 6.2.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINPAGE_H
#define UI_MAINPAGE_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainPage
{
public:
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *topMenu;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *icon;
    QSpacerItem *horizontalSpacer_2;
    QToolButton *tb_setting;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_3;
    QGroupBox *leftMunu;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_3;
    QToolButton *tb_folder;
    QSpacerItem *verticalSpacer_4;
    QToolButton *tb_transmit;
    QSpacerItem *verticalSpacer_5;
    QToolButton *tb_friend;
    QSpacerItem *verticalSpacer;
    QToolButton *tb_logout;
    QSpacerItem *verticalSpacer_2;
    QStackedWidget *funcPanel;
    QWidget *page;
    QWidget *page_2;

    void setupUi(QWidget *MainPage)
    {
        if (MainPage->objectName().isEmpty())
            MainPage->setObjectName(QString::fromUtf8("MainPage"));
        MainPage->resize(836, 470);
        horizontalLayout_4 = new QHBoxLayout(MainPage);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        topMenu = new QGroupBox(MainPage);
        topMenu->setObjectName(QString::fromUtf8("topMenu"));
        horizontalLayout_2 = new QHBoxLayout(topMenu);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(5, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        icon = new QLabel(topMenu);
        icon->setObjectName(QString::fromUtf8("icon"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(icon->sizePolicy().hasHeightForWidth());
        icon->setSizePolicy(sizePolicy);
        icon->setMinimumSize(QSize(50, 50));
        icon->setMaximumSize(QSize(50, 50));
        icon->setPixmap(QPixmap(QString::fromUtf8(":/img/res/img/icon.png")));
        icon->setScaledContents(true);
        icon->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(icon);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        tb_setting = new QToolButton(topMenu);
        tb_setting->setObjectName(QString::fromUtf8("tb_setting"));
        tb_setting->setCursor(QCursor(Qt::PointingHandCursor));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/img/res/img/setting.png"), QSize(), QIcon::Normal, QIcon::On);
        tb_setting->setIcon(icon1);
        tb_setting->setIconSize(QSize(25, 25));

        horizontalLayout->addWidget(tb_setting);

        horizontalSpacer_3 = new QSpacerItem(5, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);


        horizontalLayout_2->addLayout(horizontalLayout);


        verticalLayout_3->addWidget(topMenu);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        leftMunu = new QGroupBox(MainPage);
        leftMunu->setObjectName(QString::fromUtf8("leftMunu"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(leftMunu->sizePolicy().hasHeightForWidth());
        leftMunu->setSizePolicy(sizePolicy1);
        leftMunu->setMinimumSize(QSize(58, 0));
        leftMunu->setMaximumSize(QSize(58, 16777215));
        verticalLayout_2 = new QVBoxLayout(leftMunu);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer_3 = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_3);

        tb_folder = new QToolButton(leftMunu);
        tb_folder->setObjectName(QString::fromUtf8("tb_folder"));
        tb_folder->setCursor(QCursor(Qt::PointingHandCursor));
        tb_folder->setAutoFillBackground(false);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/img/res/img/folder_unselected.png"), QSize(), QIcon::Normal, QIcon::Off);
        tb_folder->setIcon(icon2);
        tb_folder->setIconSize(QSize(30, 30));

        verticalLayout->addWidget(tb_folder);

        verticalSpacer_4 = new QSpacerItem(20, 3, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_4);

        tb_transmit = new QToolButton(leftMunu);
        tb_transmit->setObjectName(QString::fromUtf8("tb_transmit"));
        tb_transmit->setCursor(QCursor(Qt::PointingHandCursor));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/img/res/img/transmit_unselected.png"), QSize(), QIcon::Normal, QIcon::On);
        tb_transmit->setIcon(icon3);
        tb_transmit->setIconSize(QSize(30, 30));

        verticalLayout->addWidget(tb_transmit);

        verticalSpacer_5 = new QSpacerItem(20, 3, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_5);

        tb_friend = new QToolButton(leftMunu);
        tb_friend->setObjectName(QString::fromUtf8("tb_friend"));
        tb_friend->setCursor(QCursor(Qt::PointingHandCursor));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/img/res/img/friend_unselected.png"), QSize(), QIcon::Normal, QIcon::On);
        tb_friend->setIcon(icon4);
        tb_friend->setIconSize(QSize(30, 30));

        verticalLayout->addWidget(tb_friend);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        tb_logout = new QToolButton(leftMunu);
        tb_logout->setObjectName(QString::fromUtf8("tb_logout"));
        tb_logout->setCursor(QCursor(Qt::PointingHandCursor));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/img/res/img/logout.png"), QSize(), QIcon::Normal, QIcon::On);
        tb_logout->setIcon(icon5);
        tb_logout->setIconSize(QSize(30, 30));

        verticalLayout->addWidget(tb_logout);

        verticalSpacer_2 = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_2);


        verticalLayout_2->addLayout(verticalLayout);


        horizontalLayout_3->addWidget(leftMunu);

        funcPanel = new QStackedWidget(MainPage);
        funcPanel->setObjectName(QString::fromUtf8("funcPanel"));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        funcPanel->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        funcPanel->addWidget(page_2);

        horizontalLayout_3->addWidget(funcPanel);


        verticalLayout_3->addLayout(horizontalLayout_3);


        horizontalLayout_4->addLayout(verticalLayout_3);


        retranslateUi(MainPage);

        QMetaObject::connectSlotsByName(MainPage);
    } // setupUi

    void retranslateUi(QWidget *MainPage)
    {
        MainPage->setWindowTitle(QCoreApplication::translate("MainPage", "Form", nullptr));
        topMenu->setTitle(QString());
        icon->setText(QString());
        tb_setting->setText(QCoreApplication::translate("MainPage", "...", nullptr));
        leftMunu->setTitle(QString());
        tb_folder->setText(QString());
        tb_transmit->setText(QCoreApplication::translate("MainPage", "...", nullptr));
        tb_friend->setText(QCoreApplication::translate("MainPage", "...", nullptr));
        tb_logout->setText(QCoreApplication::translate("MainPage", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainPage: public Ui_MainPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINPAGE_H
