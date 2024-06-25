#ifndef BUBBLETIPS_H
#define BUBBLETIPS_H

#include <QColor>
#include <QLabel>
#include <QPaintEvent>
#include <QString>
#include <QFont>
#include <QHBoxLayout>

class BubbleTips : public QWidget
{
    Q_OBJECT
public:
    explicit BubbleTips(QString showStr, int sec = 1, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void InitLayout();//初始化窗体的布局和部件
    QHBoxLayout *hBoxlayout;//布局显示控件布局
    QLabel *mText;//用于显示文字的控件
    QString showStr;//显示的字符串

    double opacityValue;//窗体初始化透明度

    QColor  backgroundColor;//窗体的背景色
    QColor  frameColor;//边框颜色
    int     frameSize;//边框粗细大小

    int     showTime;//显示时间

    int     closeTime;//关闭需要时间
    double  closeSpeed;//窗体消失的平滑度，大小0~1

signals:

};

#endif // BUBBLETIPS_H
