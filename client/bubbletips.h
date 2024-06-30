#ifndef BUBBLETIPS_H
#define BUBBLETIPS_H

#include <QColor>
#include <QLabel>
#include <QPaintEvent>
#include <QString>
#include <QFont>
#include <QHBoxLayout>

/**
 * @brief 气泡消息提示
 */
class BubbleTips : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 生成一个气泡消息
     * @param showStr 消息内容
     * @param sec 持续时间
     * @param parent
     * @param flag 是否设置为模态窗口
     */
    static void showBubbleTips(QString showStr, int sec = 1, QWidget *parent = nullptr, bool flag = false);

    explicit BubbleTips(QString showStr, int sec = 1, QWidget *parent = nullptr, bool flag = false);

    ~BubbleTips();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void initTimer();//初始化窗体的布局和部件
    QTimer *closeTimer;
    QTimer *singleShotTimer;
    QHBoxLayout *hBoxlayout;
    QLabel *mText;
    QString showStr;

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
