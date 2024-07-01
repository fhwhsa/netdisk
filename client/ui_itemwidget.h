/********************************************************************************
** Form generated from reading UI file 'itemwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.2.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ITEMWIDGET_H
#define UI_ITEMWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ItemWIdget
{
public:
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *left_top;
    QLabel *left_bottom;
    QSpacerItem *horizontalSpacer;
    QFrame *line;
    QLabel *right;

    void setupUi(QWidget *ItemWIdget)
    {
        if (ItemWIdget->objectName().isEmpty())
            ItemWIdget->setObjectName(QString::fromUtf8("ItemWIdget"));
        ItemWIdget->resize(504, 60);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ItemWIdget->sizePolicy().hasHeightForWidth());
        ItemWIdget->setSizePolicy(sizePolicy);
        ItemWIdget->setMinimumSize(QSize(0, 60));
        ItemWIdget->setMaximumSize(QSize(16777215, 60));
        horizontalLayout_2 = new QHBoxLayout(ItemWIdget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        left_top = new QLabel(ItemWIdget);
        left_top->setObjectName(QString::fromUtf8("left_top"));
        QFont font;
        font.setPointSize(12);
        left_top->setFont(font);

        verticalLayout->addWidget(left_top);

        left_bottom = new QLabel(ItemWIdget);
        left_bottom->setObjectName(QString::fromUtf8("left_bottom"));
        QFont font1;
        font1.setPointSize(8);
        left_bottom->setFont(font1);

        verticalLayout->addWidget(left_bottom);


        horizontalLayout->addLayout(verticalLayout);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        line = new QFrame(ItemWIdget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(line);

        right = new QLabel(ItemWIdget);
        right->setObjectName(QString::fromUtf8("right"));
        right->setFont(font);

        horizontalLayout->addWidget(right);


        horizontalLayout_2->addLayout(horizontalLayout);


        retranslateUi(ItemWIdget);

        QMetaObject::connectSlotsByName(ItemWIdget);
    } // setupUi

    void retranslateUi(QWidget *ItemWIdget)
    {
        ItemWIdget->setWindowTitle(QCoreApplication::translate("ItemWIdget", "Form", nullptr));
        left_top->setText(QCoreApplication::translate("ItemWIdget", "null", nullptr));
        left_bottom->setText(QCoreApplication::translate("ItemWIdget", "null", nullptr));
        right->setText(QCoreApplication::translate("ItemWIdget", "null", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ItemWIdget: public Ui_ItemWIdget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ITEMWIDGET_H
