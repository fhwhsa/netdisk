#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;

    QString ip; ///< 服务器ip地址
    QString port; ///< 服务器端口
    QTcpSocket *socket; ///< 套接字

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
#endif // WIDGET_H
