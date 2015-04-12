#ifndef TTINNER_H
#define TTINNER_H

#include <QThread>
#include <QTime>

#define RTIME    30

class TTInner : public QThread
{
Q_OBJECT
public:
    TTInner();
    void run();
    void touch();
signals:
    void saveNow();
    void timerDied();
private:
    QTime stoptime;
    QTime createStopTime();
};

#endif // TTINNER_H
