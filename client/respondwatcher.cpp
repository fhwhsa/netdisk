#include "respondwatcher.h"
#include "bubbletips.h"

#include <QEventLoop>

void RespondWatcher::createBgRw(QObject *_sender, const char *signal,
                            QString _timeoutStr, int _timeoutSec, QPoint pos,
                            std::function<void(std::shared_ptr<MsgUnit>)> _func)
{
    new RespondWatcher(_sender, signal, _timeoutStr, _timeoutSec, pos, _func);
}

RespondWatcher::RespondWatcher(QObject *_sender, const char *signal,
                               QString _timeoutStr, int _timeoutSec, QPoint _pos,
                               std::function<void(std::shared_ptr<MsgUnit>)> _func)
    : QObject{_sender},
    sender(_sender),
    timeoutStr(_timeoutStr),
    pos(_pos),
    func(_func)
{
    timer.setSingleShot(true);
    connect(&timer, &QTimer::timeout, this, &RespondWatcher::timeoutHandler);
    connect(sender, signal, this, SLOT(signalHandler(std::shared_ptr<MsgUnit>)));
    timer.start(_timeoutSec * 1000);
}

RespondWatcher::~RespondWatcher()
{
    disconnect();
}

void RespondWatcher::signalHandler(std::shared_ptr<MsgUnit> munit)
{
    if (timer.isActive())
        timer.stop();
    func(munit);
    deleteLater();
}

void RespondWatcher::timeoutHandler()
{
    BubbleTips::showBubbleTips(timeoutStr, pos, 2, sender);
    deleteLater();
}
