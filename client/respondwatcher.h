#ifndef RESPONDWATCHER_H
#define RESPONDWATCHER_H

#include "msgUnit.h"

#include <memory>
#include <QObject>
#include <QTimer>
#include <QString>
#include <QPoint>

/**
 * @brief 响应监听器，可以方便处理超时的情况
 */
class RespondWatcher : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief 创建一个响应监听器
     * @param _sender 响应信号的发送类
     * @param signal 响应信号
     * @param _timeoutStr 超时显示的提示信息
     * @param _timeoutSec 等待时长
     * @param pos 超时显示的气泡提示框的中心位置
     * @param _func 接收到信号后调用的回调函数
     */
    static void create(QObject *_sender, const char *signal,
                       QString _timeoutStr, int _timeoutSec, QPoint pos,
                       std::function<void(std::shared_ptr<MsgUnit>)> _func);

private:
    explicit RespondWatcher(QObject *_sender, const char *signal,
                            QString _timeoutStr, int _timeoutSec, QPoint _pos,
                            std::function<void(std::shared_ptr<MsgUnit>)> _func);
    ~RespondWatcher();

    QObject* sender;    
    QTimer timer;
    QPoint pos;
    QString timeoutStr;
    std::function<void(std::shared_ptr<MsgUnit> munit)> func;

private slots:
    /**
     * @brief 信号处理器
     * @param munit 响应信息体
     */
    void signalHandler(std::shared_ptr<MsgUnit> munit);

    /**
     * @brief 超时处理器
     */
    void timeoutHandler();
};

#endif // RESPONDWATCHER_H
