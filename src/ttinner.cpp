#include "ttinner.h"

TTInner::TTInner()
{
}

void TTInner::run()
{
    touch();

    while (stoptime >= QTime::currentTime())
    {
        QThread::sleep(RTIME);
        emit saveNow();
    }
    emit timerDied();
}

void TTInner::touch()
{
    stoptime = createStopTime();
}

QTime TTInner::createStopTime()
{
    QTime time = QTime::currentTime();
    time = time.addSecs(RTIME);
    return time;
}
