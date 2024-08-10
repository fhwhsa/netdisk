/********************************************************************************
** Form generated from reading UI file 'transmitpage.ui'
**
** Created by: Qt User Interface Compiler version 6.2.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRANSMITPAGE_H
#define UI_TRANSMITPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TransmitPage
{
public:
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout;
    QPushButton *uploadListBtn;
    QSpacerItem *verticalSpacer_3;
    QPushButton *downloadListBtn;
    QSpacerItem *verticalSpacer_4;
    QPushButton *finshListBtn;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer_2;
    QFrame *line;
    QSpacerItem *horizontalSpacer_3;
    QListWidget *uploadList;
    QListWidget *downloadList;
    QListWidget *finshList;

    void setupUi(QWidget *TransmitPage)
    {
        if (TransmitPage->objectName().isEmpty())
            TransmitPage->setObjectName(QString::fromUtf8("TransmitPage"));
        TransmitPage->resize(645, 374);
        horizontalLayout_2 = new QHBoxLayout(TransmitPage);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(TransmitPage);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(1, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        uploadListBtn = new QPushButton(groupBox);
        uploadListBtn->setObjectName(QString::fromUtf8("uploadListBtn"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(uploadListBtn->sizePolicy().hasHeightForWidth());
        uploadListBtn->setSizePolicy(sizePolicy);
        uploadListBtn->setMinimumSize(QSize(100, 30));
        uploadListBtn->setMaximumSize(QSize(100, 30));
        QFont font;
        font.setPointSize(10);
        uploadListBtn->setFont(font);
        uploadListBtn->setCursor(QCursor(Qt::PointingHandCursor));

        verticalLayout->addWidget(uploadListBtn);

        verticalSpacer_3 = new QSpacerItem(20, 3, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_3);

        downloadListBtn = new QPushButton(groupBox);
        downloadListBtn->setObjectName(QString::fromUtf8("downloadListBtn"));
        sizePolicy.setHeightForWidth(downloadListBtn->sizePolicy().hasHeightForWidth());
        downloadListBtn->setSizePolicy(sizePolicy);
        downloadListBtn->setMinimumSize(QSize(100, 30));
        downloadListBtn->setMaximumSize(QSize(100, 30));
        downloadListBtn->setFont(font);
        downloadListBtn->setCursor(QCursor(Qt::PointingHandCursor));

        verticalLayout->addWidget(downloadListBtn);

        verticalSpacer_4 = new QSpacerItem(20, 3, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_4);

        finshListBtn = new QPushButton(groupBox);
        finshListBtn->setObjectName(QString::fromUtf8("finshListBtn"));
        sizePolicy.setHeightForWidth(finshListBtn->sizePolicy().hasHeightForWidth());
        finshListBtn->setSizeP
        finshListBtn->setMinimumSize(QSize(100, 30));
        finshListBtn->setMaximumSize(QSize(100, 30));
        finshListBtn->setFont(font);
        finshListBtn->setCursor(QCursor(Qt::PointingHandCursor));

        verticalLayout->addWidget(finshListBtn);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout);

        horizontalSpacer_2 = new QSpacerItem(3, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        line = new QFrame(groupBox);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(line);

        horizontalSpacer_3 = new QSpacerItem(3, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        uploadList = new QListWidget(groupBox);
        uploadList->setObjectName(QString::fromUtf8("uploadList"));

        horizontalLayout->addWidget(uploadList);

        downloadList = new QListWidget(groupBox);
        downloadList->setObjectName(QString::fromUtf8("downloadList"));

        horizontalLayout->addWidget(downloadList);

        finshList = new QListWidget(groupBox);
        finshList->setObjectName(QString::fromUtf8("finshList"));

        horizontalLayout->addWidget(finshList);


        horizontalLayout_2->addWidget(groupBox);


        retranslateUi(TransmitPage);

        QMetaObject::connectSlotsByName(TransmitPage);
    } // setupUi

    void retranslateUi(QWidget *TransmitPage)
    {
        TransmitPage->setWindowTitle(QCoreApplication::translate("TransmitPage", "Form", nullptr));
        groupBox->setTitle(QString());
        uploadListBtn->setText(QCoreApplication::translate("TransmitPage", "\346\255\243\345\234\250\344\270\212\344\274\240", nullptr));
        downloadListBtn->setText(QCoreApplication::translate("TransmitPage", "\346\255\243\345\234\250\344\270\213\350\275\275", nullptr));
        finshListBtn->setText(QCoreApplication::translate("TransmitPage", "\344\274\240\350\276\223\345\256\214\346\210\220", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TransmitPage: public Ui_TransmitPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRANSMITPAGE_H
