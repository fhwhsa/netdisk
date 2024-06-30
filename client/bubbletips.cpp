#include "bubbletips.h"

#include <QTimer>
#include <QPainter>
#include <QRectF>
#include <QPoint>
#include <QDebug>

void BubbleTips::showBubbleTips(QString showStr, int sec, QWidget *parent, bool flag)
{
    BubbleTips *tips = new BubbleTips(showStr, sec, parent, flag);
    tips->setAttribute(Qt::WA_DeleteOnClose);
    tips->show();
    return;
}

BubbleTips::~BubbleTips()
{
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

BubbleTips::BubbleTips(QString showStr, int sec, QWidget *parent, bool flag): QWidget(parent),
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

    if (nullptr != parent)
    {
        QPoint pPos = parent->pos();
        this->move(pPos.rx() + parent->width() / 2 - this->width() / 2, pPos.ry() + parent->height() / 2 - this->height() / 2);
    }
    if (flag)
        this->setWindowModality(Qt::WindowModal);
}

void BubbleTips::initTimer()
{
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

    //执行延时自动关闭
    singleShotTimer = new QTimer(this);
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
