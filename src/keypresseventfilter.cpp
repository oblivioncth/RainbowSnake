#include "keypresseventfilter.h"

#include <QKeyEvent>
#include <QDebug>

KeyPressEventFilter::KeyPressEventFilter(QObject *parent) //Allows intercepting of key strokes
    : QObject(parent)
{
}

bool KeyPressEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    //----- [Initialize] -----
    if (event->type() != QEvent::KeyPress)
        return QObject::eventFilter(obj, event);

    //----- [Specific key actions] -----
    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
    switch(keyEvent->key())
    {
        case Qt::Key_Up:
        if(bGameRunning == false || SnakeDirection != South)
            bufferDirection = North;
            bGameRunning = true;
        break;
        case Qt::Key_Left:
        if(bGameRunning == false || SnakeDirection != East)
            bufferDirection = West;
            bGameRunning = true;
        break;
        case Qt::Key_Right:
        if(bGameRunning == false || SnakeDirection != West)
            bufferDirection = East;
            bGameRunning = true;
        break;
        case Qt::Key_Down:
        if(bGameRunning == false || SnakeDirection != North)
            bufferDirection = South;
            bGameRunning = true;
        break;
        case Qt::Key_Space:
            bUsingPower = true;
        break;
    }
    return true;
}

KeyPressEventFilter::Direction KeyPressEventFilter::getDirection()
{
    return SnakeDirection;
}

void KeyPressEventFilter::fnReleaseDirBuffer() //Prevents multiple direction changes in one frame which would allow an unintended 180 degree turn
{
    SnakeDirection = bufferDirection;
}
