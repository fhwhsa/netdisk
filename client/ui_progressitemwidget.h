/********************************************************************************
** Form generated from reading UI file 'progressitemwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.2.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROGRESSITEMWIDGET_H
#define UI_PROGRESSITEMWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_progressItems
{
public:
    QHBoxLayout *horizontalLayout_3;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_4;
    QLabel *filename;
    QSpacerItem *horizontalSpacer;
    QProgressBar *progressBar;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QLabel *progress;
    QToolButton *pauseOrContBtn;
    QToolButton *cancelBtn;

    void setupUi(QWidget *progressItems)
    {
        if (progressItems->objectName().isEmpty())
            progressItems->setObjectName(QString::fromUtf8("progressItems"));
        progressItems->resize(688, 42);
        horizontalLayout_3 = new QHBoxLayout(progressItems);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_4 = new QSpacerItem(13, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        filename = new QLabel(progressItems);
        filename->setObjectName(QString::fromUtf8("filename"));
        QFont font;
        font.setPointSize(10);
        filename->setFont(font);

        horizontalLayout_2->addWidget(filename);

        horizontalSpacer = new QSpacerItem(5, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        gridLayout->addLayout(horizontalLayout_2, 0, 0, 1, 1);

        progressBar = new QProgressBar(progressItems);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(progressBar->sizePolicy().hasHeightForWidth());
        progressBar->setSizePolicy(sizePolicy);
        progressBar->setMinimumSize(QSize(500, 20));
        progressBar->setMaximumSize(QSize(500, 20));
        progressBar->setValue(0);
        progressBar->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(progressBar, 0, 1, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_2 = new QSpacerItem(5, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        progress = new QLabel(progressItems);
        progress->setObjectName(QString::fromUtf8("progress"));
        progress->setFont(font);

        horizontalLayout->addWidget(progress);

        pauseOrContBtn = new QToolButton(progressItems);
        pauseOrContBtn->setObjectName(QString::fromUtf8("pauseOrContBtn"));
        pauseOrContBtn->setMinimumSize(QSize(20, 20));
        pauseOrContBtn->setMaximumSize(QSize(20, 20));
        pauseOrContBtn->setCursor(QCursor(Qt::PointingHandCursor));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/img/res/img/continue.png"), QSize(), QIcon::Normal, QIcon::Off);
        pauseOrContBtn->setIcon(icon);

        horizontalLayout->addWidget(pauseOrContBtn);

        cancelBtn = new QToolButton(progressItems);
        cancelBtn->setObjectName(QString::fromUtf8("cancelBtn"));
        cancelBtn->setMinimumSize(QSize(20, 20));
        cancelBtn->setMaximumSize(QSize(20, 20));
        cancelBtn->setCursor(QCursor(Qt::PointingHandCursor));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/img/res/img/cancel.png"), QSize(), QIcon::Normal, QIcon::Off);
        cancelBtn->setIcon(icon1);

        horizontalLayout->addWidget(cancelBtn);


        gridLayout->addLayout(horizontalLayout, 0, 2, 1, 1);


        horizontalLayout_3->addLayout(gridLayout);


        retranslateUi(progressItems);

        QMetaObject::connectSlotsByName(progressItems);
    } // setupUi

    void retranslateUi(QWidget *progressItems)
    {
        progressItems->setWindowTitle(QCoreApplication::translate("progressItems", "Form", nullptr));
        filename->setText(QCoreApplication::translate("progressItems", "\346\226\207\344\273\266\345\220\215", nullptr));
        progress->setText(QCoreApplication::translate("progressItems", "0/0", nullptr));
        pauseOrContBtn->setText(QCoreApplication::translate("progressItems", "...", nullptr));
        cancelBtn->setText(QCoreApplication::translate("progressItems", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class progressItems: public Ui_progressItems {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROGRESSITEMWIDGET_H
