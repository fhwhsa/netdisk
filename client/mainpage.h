#ifndef MAINPAGE_H
#define MAINPAGE_H

#include "folderpage.h"
#include "transmitpage.h"
#include "friendpage.h"

#include <QWidget>
#include <QHash>
#include <QToolButton>

namespace Ui {
class MainPage;
}

class MainPage : public QWidget
{
    Q_OBJECT

public:
    explicit MainPage(QWidget *parent = nullptr);
    ~MainPage();

private:
    Ui::MainPage *ui;

    /**
     * @brief 初始化
     */
    void init();

    /**
     * @brief 初始化信号与槽
     */
    void iniSignalSlots();

    QHash<QToolButton*, QPair<QIcon, QIcon>> btnIcon;   ///< 存储按钮选中/未选中的图标地址
    QToolButton* currSelectedBtn;   ///< 记录当前选中的图标

    FolderPage* folderPage;
    TransmitPage* transmitPage;
    FriendPage* friendPage;

private slots:
    void clickTbfolder();
    void clickTbtransmit();
    void clickTbfriend();
    void clickTblogout();
    void clickTbsetting();
};

#endif // MAINPAGE_H
