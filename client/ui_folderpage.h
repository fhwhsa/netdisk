/********************************************************************************
** Form generated from reading UI file 'folderpage.ui'
**
** Created by: Qt User Interface Compiler version 6.2.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FOLDERPAGE_H
#define UI_FOLDERPAGE_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FolderPage
{
public:
    QVBoxLayout *verticalLayout_4;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QListWidget *fileList;
    QVBoxLayout *verticalLayout;
    QFrame *line;
    QToolButton *tb_back;
    QToolButton *tb_flush;
    QFrame *line_2;
    QToolButton *tb_upload;
    QToolButton *tb_download;
    QFrame *line_3;
    QToolButton *tb_addfolder;
    QToolButton *tb_rename;
    QToolButton *tb_delete;
    QFrame *line_5;
    QToolButton *tb_share;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *FolderPage)
    {
        if (FolderPage->objectName().isEmpty())
            FolderPage->setObjectName(QString::fromUtf8("FolderPage"));
        FolderPage->resize(733, 498);
        verticalLayout_4 = new QVBoxLayout(FolderPage);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(FolderPage);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_3 = new QVBoxLayout(groupBox);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        fileList = new QListWidget(groupBox);
        fileList->setObjectName(QString::fromUtf8("fileList"));

        horizontalLayout_2->addWidget(fileList);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        line = new QFrame(groupBox);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        tb_back = new QToolButton(groupBox);
        tb_back->setObjectName(QString::fromUtf8("tb_back"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/img/res/img/back.png"), QSize(), QIcon::Normal, QIcon::Off);
        tb_back->setIcon(icon);

        verticalLayout->addWidget(tb_back);

        tb_flush = new QToolButton(groupBox);
        tb_flush->setObjectName(QString::fromUtf8("tb_flush"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/img/res/img/flush.png"), QSize(), QIcon::Normal, QIcon::Off);
        tb_flush->setIcon(icon1);

        verticalLayout->addWidget(tb_flush);

        line_2 = new QFrame(groupBox);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_2);

        tb_upload = new QToolButton(groupBox);
        tb_upload->setObjectName(QString::fromUtf8("tb_upload"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/img/res/img/upload.png"), QSize(), QIcon::Normal, QIcon::Off);
        tb_upload->setIcon(icon2);

        verticalLayout->addWidget(tb_upload);

        tb_download = new QToolButton(groupBox);
        tb_download->setObjectName(QString::fromUtf8("tb_download"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/img/res/img/download.png"), QSize(), QIcon::Normal, QIcon::Off);
        tb_download->setIcon(icon3);

        verticalLayout->addWidget(tb_download);

        line_3 = new QFrame(groupBox);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_3);

        tb_addfolder = new QToolButton(groupBox);
        tb_addfolder->setObjectName(QString::fromUtf8("tb_addfolder"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/img/res/img/addfolder.png"), QSize(), QIcon::Normal, QIcon::Off);
        tb_addfolder->setIcon(icon4);

        verticalLayout->addWidget(tb_addfolder);

        tb_rename = new QToolButton(groupBox);
        tb_rename->setObjectName(QString::fromUtf8("tb_rename"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/img/res/img/rename.png"), QSize(), QIcon::Normal, QIcon::Off);
        tb_rename->setIcon(icon5);

        verticalLayout->addWidget(tb_rename);

        tb_delete = new QToolButton(groupBox);
        tb_delete->setObjectName(QString::fromUtf8("tb_delete"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/img/res/img/delete.png"), QSize(), QIcon::Normal, QIcon::Off);
        tb_delete->setIcon(icon6);

        verticalLayout->addWidget(tb_delete);

        line_5 = new QFrame(groupBox);
        line_5->setObjectName(QString::fromUtf8("line_5"));
        line_5->setFrameShape(QFrame::HLine);
        line_5->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_5);

        tb_share = new QToolButton(groupBox);
        tb_share->setObjectName(QString::fromUtf8("tb_share"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/img/res/img/share.png"), QSize(), QIcon::Normal, QIcon::Off);
        tb_share->setIcon(icon7);

        verticalLayout->addWidget(tb_share);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout_2->addLayout(verticalLayout);


        verticalLayout_2->addLayout(horizontalLayout_2);


        verticalLayout_3->addLayout(verticalLayout_2);


        verticalLayout_4->addWidget(groupBox);


        retranslateUi(FolderPage);

        QMetaObject::connectSlotsByName(FolderPage);
    } // setupUi

    void retranslateUi(QWidget *FolderPage)
    {
        FolderPage->setWindowTitle(QCoreApplication::translate("FolderPage", "Form", nullptr));
        groupBox->setTitle(QString());
        tb_back->setText(QCoreApplication::translate("FolderPage", "...", nullptr));
#if QT_CONFIG(tooltip)
        tb_flush->setToolTip(QCoreApplication::translate("FolderPage", "\345\210\267\346\226\260", nullptr));
#endif // QT_CONFIG(tooltip)
        tb_flush->setText(QCoreApplication::translate("FolderPage", "...", nullptr));
#if QT_CONFIG(tooltip)
        tb_upload->setToolTip(QCoreApplication::translate("FolderPage", "\344\270\212\344\274\240", nullptr));
#endif // QT_CONFIG(tooltip)
        tb_upload->setText(QCoreApplication::translate("FolderPage", "...", nullptr));
#if QT_CONFIG(tooltip)
        tb_download->setToolTip(QCoreApplication::translate("FolderPage", "\344\270\213\350\275\275", nullptr));
#endif // QT_CONFIG(tooltip)
        tb_download->setText(QCoreApplication::translate("FolderPage", "...", nullptr));
#if QT_CONFIG(tooltip)
        tb_addfolder->setToolTip(QCoreApplication::translate("FolderPage", "\346\226\260\345\273\272\346\226\207\344\273\266\345\244\271", nullptr));
#endif // QT_CONFIG(tooltip)
        tb_addfolder->setText(QCoreApplication::translate("FolderPage", "...", nullptr));
#if QT_CONFIG(tooltip)
        tb_rename->setToolTip(QCoreApplication::translate("FolderPage", "\351\207\215\345\221\275\345\220\215", nullptr));
#endif // QT_CONFIG(tooltip)
        tb_rename->setText(QCoreApplication::translate("FolderPage", "...", nullptr));
#if QT_CONFIG(tooltip)
        tb_delete->setToolTip(QCoreApplication::translate("FolderPage", "\345\210\240\351\231\244", nullptr));
#endif // QT_CONFIG(tooltip)
        tb_delete->setText(QCoreApplication::translate("FolderPage", "...", nullptr));
#if QT_CONFIG(tooltip)
        tb_share->setToolTip(QCoreApplication::translate("FolderPage", "\345\210\206\344\272\253", nullptr));
#endif // QT_CONFIG(tooltip)
        tb_share->setText(QCoreApplication::translate("FolderPage", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FolderPage: public Ui_FolderPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FOLDERPAGE_H
