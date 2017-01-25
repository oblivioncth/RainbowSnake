#ifndef KEYPRESSEVENTFILTER_H
#define KEYPRESSEVENTFILTER_H

#include <QObject>

class KeyPressEventFilter : public QObject
{
    Q_OBJECT

public:
    explicit KeyPressEventFilter(QObject *parent = nullptr);
    enum Direction {North,South,East,West};
    Direction getDirection();
    void fnReleaseDirBuffer();

    //Game states
    bool bGameRunning = false;
    bool bGameLost = false;
    bool bUsingPower = false;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    Direction SnakeDirection;
    Direction bufferDirection;
};

#endif // KEYPRESSEVENTFILTER_H
