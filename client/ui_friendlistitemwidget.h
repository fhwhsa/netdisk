/********************************************************************************
** Form generated from reading UI file 'friendlistitemwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.2.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRIENDLISTITEMWIDGET_H
#define UI_FRIENDLISTITEMWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FListItemWidget
{
public:
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *leftLabel;
    QSpacerItem *horizontalSpacer;
    QFrame *line;
    QLabel *rightLabel;

    void setupUi(QWidget *FListItemWidget)
    {
        if (FListItemWidget->objectName().isEmpty())
            FListItemWidget->setObjectName(QString::fromUtf8("FListItemWidget"));
        FListItemWidget->resize(545, 50);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(FListItemWidget->sizePolicy().hasHeightForWidth());
        FListItemWidget->setSizePolicy(sizePolicy);
        FListItemWidget->setMinimumSize(QSize(0, 50));
        FListItemWidget->setMaximumSize(QSize(16777215, 50));
        horizontalLayout_2 = new QHBoxLayout(FListItemWidget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        leftLabel = new QLabel(FListItemWidget);
        leftLabel->setObjectName(QString::fromUtf8("leftLabel"));
        QFont font;
        font.setPointSize(12);
        leftLabel->setFont(font);

        horizontalLayout->addWidget(leftLabel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        line = new QFrame(FListItemWidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(line);

        rightLabel = new QLabel(FListItemWidget);
        rightLabel->setObjectName(QString::fromUtf8("rightLabel"));
        QFont font1;
        font1.setPointSize(8);
        rightLabel->setFont(font1);

        horizontalLayout->addWidget(rightLabel);


        horizontalLayout_2->addLayout(horizontalLayout);


        retranslateUi(FListItemWidget);

        QMetaObject::connectSlotsByName(FListItemWidget);
    } // setupUi

    void retranslateUi(QWidget *FListItemWidget)
    {
        FListItemWidget->setWindowTitle(QCoreApplication::translate("FListItemWidget", "Form", nullptr));
        leftLabel->setText(QCoreApplication::translate("FListItemWidget", "TextLabel", nullptr));
        rightLabel->setText(QCoreApplication::translate("FListItemWidget", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FListItemWidget: public Ui_FListItemWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRIENDLISTITEMWIDGET_H
