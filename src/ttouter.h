#ifndef TTOUTER_H
#define TTOUTER_H

#include "ttinner.h"
#include <QObject>

class TTOuter : public QObject
{
Q_OBJECT
public:
    explicit TTOuter(QObject *parent = 0);

signals:
    void saveNow();
public slots:
    void keyPressed();
    void timerDied();
    void handleSaveNow();
    void closeDown();
private:
    TTInner *timer;
};

#endif // TTOUTER_H
