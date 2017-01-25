#include "snakeblock.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>

SnakeBlock::SnakeBlock(bool bHead) //Linked-list node
{
    if(bHead)
        iColor = 0;
    else
        iColor = qrand() % 4;
}

QRectF SnakeBlock::boundingRect() const //Used for painter overlap prevention with SnakeWall
{
    return QRectF(-19, -19, 18, 18);
}

QPainterPath SnakeBlock::shape() const //Kept for compatability with QGraphicsScene but unused
{
    QPainterPath path;
    path.addRect(-20, -20, 19, 19);
    return path;
}

void SnakeBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //----- [Apply block color & draw] -----
    switch(iColor)
    {
        case 0:
        painter->setBrush(Qt::red);
        break;

        case 1:
        painter->setBrush(Qt::green);
        break;

        case 2:
        painter->setBrush(Qt::blue);
        break;

        case 3:
        painter->setBrush(Qt::yellow);
        break;
    }
    painter->drawRect(-20, -20, 19, 19);
}

int SnakeBlock::fnGetColor()
{
    return iColor;
}

void SnakeBlock::fnSetColor(int iNewColor)
{
    if(iNewColor < 0 || iNewColor > 3) //Checks for valid color
        throw std::invalid_argument( "Invalid color specified" );
    iColor = iNewColor;
}
