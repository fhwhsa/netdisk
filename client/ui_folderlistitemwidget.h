/********************************************************************************
** Form generated from reading UI file 'folderlistitemwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.2.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FOLDERLISTITEMWIDGET_H
#define UI_FOLDERLISTITEMWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FolderListItemWidget
{
public:
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *icon;
    QSpacerItem *horizontalSpacer_2;
    QLabel *name;
    QSpacerItem *horizontalSpacer_3;
    QLabel *size;
    QSpacerItem *horizontalSpacer_4;

    void setupUi(QWidget *FolderListItemWidget)
    {
        if (FolderListItemWidget->objectName().isEmpty())
            FolderListItemWidget->setObjectName(QString::fromUtf8("FolderListItemWidget"));
        FolderListItemWidget->resize(731, 70);
        horizontalLayout_2 = new QHBoxLayout(FolderListItemWidget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(5, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        icon = new QLabel(FolderListItemWidget);
        icon->setObjectName(QString::fromUtf8("icon"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(icon->sizePolicy().hasHeightForWidth());
        icon->setSizePolicy(sizePolicy);
        icon->setMinimumSize(QSize(50, 50));
        icon->setMaximumSize(QSize(50, 50));

        horizontalLayout->addWidget(icon);

        horizontalSpacer_2 = new QSpacerItem(5, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        name = new QLabel(FolderListItemWidget);
        name->setObjectName(QString::fromUtf8("name"));

        horizontalLayout->addWidget(name);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        size = new QLabel(FolderListItemWidget);
        size->setObjectName(QString::fromUtf8("size"));

        horizontalLayout->addWidget(size);

        horizontalSpacer_4 = new QSpacerItem(5, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);


        horizontalLayout_2->addLayout(horizontalLayout);


        retranslateUi(FolderListItemWidget);

        QMetaObject::connectSlotsByName(FolderListItemWidget);
    } // setupUi

    void retranslateUi(QWidget *FolderListItemWidget)
    {
        FolderListItemWidget->setWindowTitle(QCoreApplication::translate("FolderListItemWidget", "Form", nullptr));
        icon->setText(QString());
        name->setText(QCoreApplication::translate("FolderListItemWidget", "name", nullptr));
        size->setText(QCoreApplication::translate("FolderListItemWidget", "size", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FolderListItemWidget: public Ui_FolderListItemWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FOLDERLISTITEMWIDGET_H
