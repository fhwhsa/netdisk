#ifndef LOGIN_H
#define LOGIN_H

#include "mainpage.h"

#include <QWidget>
#include <QString>
#include <QTcpSocket>

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private:
    Ui::Login *ui;

    QString ip; ///< 服务器ip地址
    QString port; ///< 服务器端口
    QTcpSocket *socket; ///< 套接字
    MainPage *mainPage;

    /**
     * @brief 初始化
     */
    void init();

    /**
     * @brief 初始化信号绑定
     */
    void iniSignalSlots();

    /**
     * @brief 加载服务器地址信息
     */
    void loadconfig();

    /**
     * @brief 连接服务器
     * @return true表示连接成功
     */
    bool connectServer();


private slots:
    /**
     * @brief 登陆
     */
    void login();

    /**
     * @brief 注册
     */
    void regist();

};

#endif // LOGIN_H
