#ifndef SNAKEWALL_H
#define SNAKEWALL_H

#include <QGraphicsItem>
#include "snakeblock.h"
#include "keypresseventfilter.h"

class SnakeWall : public QGraphicsItem
{
public:
    SnakeWall();

    struct WallSettings
    {
        int iXPos;
        int iYPos;
        int iRot;
    };

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) Q_DECL_OVERRIDE;
    WallSettings fnMatchWall(SnakeBlock *pHead,KeyPressEventFilter::Direction headDir);
    int fnGetWallState();
    int fnGetFacingColor(KeyPressEventFilter::Direction headDir);
protected:

private:
    int iWallState = 0;

};
#endif // SNAKEWALL_H
