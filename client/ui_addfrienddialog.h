/********************************************************************************
** Form generated from reading UI file 'addfrienddialog.ui'
**
** Created by: Qt User Interface Compiler version 6.2.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDFRIENDDIALOG_H
#define UI_ADDFRIENDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AddFriendDialog
{
public:
    QHBoxLayout *horizontalLayout_6;
    QVBoxLayout *verticalLayout;
    QGroupBox *inputBox;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *targetKey;
    QFrame *line;
    QToolButton *tb_search;
    QGroupBox *resultBox;
    QHBoxLayout *horizontalLayout_5;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer;
    QLabel *resultEmail;
    QFrame *line_2;
    QToolButton *tb_add;
    QSpacerItem *horizontalSpacer_2;
    QGroupBox *errorBox;
    QHBoxLayout *horizontalLayout_3;
    QLabel *errInfo;

    void setupUi(QDialog *AddFriendDialog)
    {
        if (AddFriendDialog->objectName().isEmpty())
            AddFriendDialog->setObjectName(QString::fromUtf8("AddFriendDialog"));
        AddFriendDialog->resize(515, 257);
        AddFriendDialog->setMinimumSize(QSize(0, 30));
        horizontalLayout_6 = new QHBoxLayout(AddFriendDialog);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        inputBox = new QGroupBox(AddFriendDialog);
        inputBox->setObjectName(QString::fromUtf8("inputBox"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(inputBox->sizePolicy().hasHeightForWidth());
        inputBox->setSizePolicy(sizePolicy);
        inputBox->setMinimumSize(QSize(0, 70));
        inputBox->setMaximumSize(QSize(16777215, 70));
        horizontalLayout_2 = new QHBoxLayout(inputBox);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(inputBox);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        label->setMinimumSize(QSize(20, 20));
        label->setMaximumSize(QSize(20, 20));
        label->setAutoFillBackground(false);
        label->setPixmap(QPixmap(QString::fromUtf8(":/img/res/img/search.png")));
        label->setScaledContents(true);
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label);

        targetKey = new QLineEdit(inputBox);
        targetKey->setObjectName(QString::fromUtf8("targetKey"));
        targetKey->setMinimumSize(QSize(0, 25));
        targetKey->setMaximumSize(QSize(16777215, 25));
        targetKey->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout->addWidget(targetKey);

        line = new QFrame(inputBox);
        line->setObjectName(QString::fromUtf8("line"));
        line->setMaximumSize(QSize(16777215, 25));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(line);

        tb_search = new QToolButton(inputBox);
        tb_search->setObjectName(QString::fromUtf8("tb_search"));
        tb_search->setMinimumSize(QSize(35, 25));
        tb_search->setMaximumSize(QSize(35, 25));
        QFont font;
        font.setPointSize(10);
        tb_search->setFont(font);
        tb_search->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout->addWidget(tb_search);


        horizontalLayout_2->addLayout(horizontalLayout);


        verticalLayout->addWidget(inputBox);

        resultBox = new QGroupBox(AddFriendDialog);
        resultBox->setObjectName(QString::fromUtf8("resultBox"));
        sizePolicy.setHeightForWidth(resultBox->sizePolicy().hasHeightForWidth());
        resultBox->setSizePolicy(sizePolicy);
        resultBox->setMinimumSize(QSize(0, 50));
        resultBox->setMaximumSize(QSize(16777215, 70));
        horizontalLayout_5 = new QHBoxLayout(resultBox);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer = new QSpacerItem(5, 5, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        resultEmail = new QLabel(resultBox);
        resultEmail->setObjectName(QString::fromUtf8("resultEmail"));

        horizontalLayout_4->addWidget(resultEmail);

        line_2 = new QFrame(resultBox);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);

        horizontalLayout_4->addWidget(line_2);

        tb_add = new QToolButton(resultBox);
        tb_add->setObjectName(QString::fromUtf8("tb_add"));
        tb_add->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout_4->addWidget(tb_add);

        horizontalSpacer_2 = new QSpacerItem(5, 5, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);


        horizontalLayout_5->addLayout(horizontalLayout_4);


        verticalLayout->addWidget(resultBox);

        errorBox = new QGroupBox(AddFriendDialog);
        errorBox->setObjectName(QString::fromUtf8("errorBox"));
        sizePolicy.setHeightForWidth(errorBox->sizePolicy().hasHeightForWidth());
        errorBox->setSizePolicy(sizePolicy);
        errorBox->setMinimumSize(QSize(0, 50));
        errorBox->setMaximumSize(QSize(16777215, 50));
        horizontalLayout_3 = new QHBoxLayout(errorBox);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        errInfo = new QLabel(errorBox);
        errInfo->setObjectName(QString::fromUtf8("errInfo"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(errInfo->sizePolicy().hasHeightForWidth());
        errInfo->setSizePolicy(sizePolicy2);
        errInfo->setMinimumSize(QSize(0, 0));
        errInfo->setMaximumSize(QSize(16777215, 16777215));
        errInfo->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(errInfo);


        verticalLayout->addWidget(errorBox);


        horizontalLayout_6->addLayout(verticalLayout);


        retranslateUi(AddFriendDialog);

        QMetaObject::connectSlotsByName(AddFriendDialog);
    } // setupUi

    void retranslateUi(QDialog *AddFriendDialog)
    {
        AddFriendDialog->setWindowTitle(QCoreApplication::translate("AddFriendDialog", "Dialog", nullptr));
        inputBox->setTitle(QCoreApplication::translate("AddFriendDialog", "\350\276\223\345\205\245\350\264\246\345\217\267\351\202\256\347\256\261/id", nullptr));
        label->setText(QString());
        targetKey->setPlaceholderText(QCoreApplication::translate("AddFriendDialog", "\350\264\246\345\217\267\351\202\256\347\256\261/id", nullptr));
        tb_search->setText(QCoreApplication::translate("AddFriendDialog", "\346\220\234\347\264\242", nullptr));
        resultBox->setTitle(QString());
        resultEmail->setText(QCoreApplication::translate("AddFriendDialog", "\350\264\246\345\217\267\351\202\256\347\256\261", nullptr));
        tb_add->setText(QCoreApplication::translate("AddFriendDialog", "\346\267\273\345\212\240\345\245\275\345\217\213", nullptr));
        errorBox->setTitle(QString());
        errInfo->setText(QCoreApplication::translate("AddFriendDialog", "info", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AddFriendDialog: public Ui_AddFriendDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDFRIENDDIALOG_H
