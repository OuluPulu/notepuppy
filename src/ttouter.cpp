#include "ttouter.h"

TTOuter::TTOuter(QObject *parent) :
        QObject(parent)
{
    timer = NULL;
}

void TTOuter::keyPressed()
{
    if (NULL == timer)
    {
        //create timer, connect signals to slots
        timer = new TTInner();
        connect(timer, SIGNAL(saveNow()), this, SLOT(handleSaveNow()));
        connect(timer, SIGNAL(timerDied()), this, SLOT(timerDied()));
        timer->start();
    }
    else
    {
        //touch timer
        timer->touch();
    }
}

void TTOuter::handleSaveNow()
{
    emit saveNow();
}

void TTOuter::closeDown()
{
    if (NULL != timer)
    {
        emit saveNow();
    }
}

void TTOuter::timerDied()
{
    delete timer;
    timer = NULL;
}
