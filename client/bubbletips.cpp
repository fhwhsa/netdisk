#include "bubbletips.h"

#include <QTimer>
#include <QPainter>
#include <QRectF>
#include <QPoint>
#include <QDebug>

void BubbleTips::showBubbleTips(QString showStr, int sec, QWidget *parent)
{
    BubbleTips *tips = new BubbleTips(showStr, sec, parent);
    tips->show();
    return;
}

BubbleTips::~BubbleTips()
{
    singleShotTimer->stop();
    closeTimer->stop();

    if (nullptr != closeTimer)
        delete closeTimer;
    if (nullptr != singleShotTimer)
        delete singleShotTimer;
}

BubbleTips::BubbleTips(QString showStr, int sec, QWidget *parent): QWidget(parent),
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
    this->setAttribute(Qt::WA_DeleteOnClose);
    hBoxlayout->addWidget(mText);
    InitLayout();

    if (nullptr != parent)
    {
        QPoint pPos = parent->pos();
        this->move(pPos.rx() + parent->width() / 2 - this->width() / 2, pPos.ry() + parent->height() / 2 - this->height() / 2);
    }
}

void BubbleTips::InitLayout()
{
    this->setWindowOpacity(opacityValue);

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
