#include "bubbletips.h"

#include <QTimer>
#include <QPainter>
#include <QRectF>
#include <QPoint>
#include <QDebug>
#include <QApplication>

void BubbleTips::showBubbleTips(QString showStr, int sec, QWidget *parent, bool flag)
{
    BubbleTips* tips = new BubbleTips(showStr, flag, sec, parent);
    tips->setAttribute(Qt::WA_DeleteOnClose);
    tips->show();
    return;
}

void BubbleTips::showBubbleTips(QString showStr, QPoint pos, int sec, QObject *father, bool flag)
{
    BubbleTips* tips = new BubbleTips(showStr, pos, flag, sec, father);
    tips->setAttribute(Qt::WA_DeleteOnClose);
    tips->show();
    return;
}

BubbleTips::~BubbleTips()
{
//    qDebug() << "delete";
    if (nullptr != closeTimer)
    {
        closeTimer->stop();
        delete closeTimer;
    }
    if (nullptr != singleShotTimer)
    {
        singleShotTimer->stop();
        delete singleShotTimer;
    }
}

BubbleTips::BubbleTips(QString showStr, bool flag, int sec, QWidget *parent): QWidget(parent),
    opacityValue(0.9),
    backgroundColor(QColor(255,255,255)),
    frameColor(QColor(255,255,255)),
    frameSize(2),
    showTime(sec * 1000),
    closeTime(50),
    closeSpeed(0.1),
    hBoxlayout(new QHBoxLayout(this)),
    mText(new QLabel(showStr, this))
{
    setWindowFlags(Qt::Window|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint|Qt::Tool|Qt::X11BypassWindowManagerHint);
    this->setAttribute(Qt::WA_TranslucentBackground); // 设置窗口部件的背景半透明
    this->setAttribute(Qt::WA_TransparentForMouseEvents, true);// 禁止鼠标事件
//    this->setAttribute(Qt::WA_DeleteOnClose); // 关闭时删除对象
    hBoxlayout->addWidget(mText);
    this->setWindowOpacity(opacityValue);
    initTimer();

    QPoint pPos;
    QSize pSize;
    QWidget* aw = QApplication::activeWindow();
    if (nullptr != aw) // 检查是否有激活的窗口，如果有则将气泡消息显示在激活窗口中央
    {
        pPos = QApplication::activeWindow()->pos();
        pSize = QApplication::activeWindow()->size();
    }
    else if (parent != nullptr) // 没有激活窗口，如果有传入QWidget对象则显示在其中间
    {
        pPos = parent->pos();
        pSize = parent->size();
    }
    else // 显示在电脑全屏中间
    {
        QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
        pPos = QPoint(screenGeometry.width() / 2, screenGeometry.height() / 2);
    }

    pPos += QPoint(pSize.width() / 2, pSize.height() / 2);
    this->move(pPos - QPoint(this->width() / 2, this->height() / 2));

    if (flag)
        this->setWindowModality(Qt::WindowModal);
}

BubbleTips::BubbleTips(QString showStr, QPoint pos, bool flag, int sec, QObject* father): QWidget(nullptr),
    opacityValue(0.9),
    backgroundColor(QColor(255,255,255)),
    frameColor(QColor(255,255,255)),
    frameSize(2),
    showTime(sec * 1000),
    closeTime(50),
    closeSpeed(0.1),
    hBoxlayout(new QHBoxLayout(this)),
    mText(new QLabel(showStr, this))
{
    setWindowFlags(Qt::Window|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint|Qt::Tool|Qt::X11BypassWindowManagerHint);
    this->setAttribute(Qt::WA_TranslucentBackground); // 设置窗口部件的背景半透明
    this->setAttribute(Qt::WA_TransparentForMouseEvents, true);// 禁止鼠标事件
    //    this->setAttribute(Qt::WA_DeleteOnClose); // 关闭时删除对象
    hBoxlayout->addWidget(mText);
    this->setWindowOpacity(opacityValue);
    initTimer();

    this->move(pos.rx() - this->width() / 2, pos.ry() - this->height() / 2);
    if (nullptr != father)
        connect(father, &QObject::destroyed, this, &BubbleTips::close);

    if (flag)
        this->setWindowModality(Qt::WindowModal);
}

void BubbleTips::initTimer()
{
    // 逐渐提高透明度，提供视觉上逐渐消失的感觉
    closeTimer = new QTimer(this);
    closeTimer->setTimerType(Qt::PreciseTimer);
    connect(closeTimer,&QTimer::timeout,this,[=](){
        if(opacityValue<=0){
            closeTimer->stop();
            this->close();
            return;
        }
        opacityValue = opacityValue-closeSpeed;
        this->setWindowOpacity(opacityValue);
    });

    // 执行延时自动关闭
    singleShotTimer = new QTimer(this);
    singleShotTimer->setSingleShot(true);
    connect(singleShotTimer, &QTimer::timeout, this, [=](){
        closeTimer->start(closeTime);
    });

    singleShotTimer->start(showTime);
}

void BubbleTips::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setBrush(QBrush(backgroundColor));

    painter.setPen(QPen(frameColor,frameSize));
    QRectF rect(0, 0, this->width(), this->height());
    painter.drawRoundedRect(rect, 15, 15);
}
