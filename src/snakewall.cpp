#include "snakewall.h"
#include "snakeblock.h"
#include "keypresseventfilter.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>

SnakeWall::SnakeWall()
{

}

QRectF SnakeWall::boundingRect() const //Used for painter overlap prevention with SnakeBlock
{
    return QRectF(-19, -19, 18, 18);
}

QPainterPath SnakeWall::shape() const //Kept for compatability with QGraphicsScene but unused
{
    QPainterPath path;
    path.addRect(-20, -20, 19, 19);
    return path;
}

void SnakeWall::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //----- [Apply wall color & draw] -----
    for(int i = 1; i < 39; i++)
    {
        painter->setBrush(Qt::red);
        painter->drawRect(-20*i-40, -820, 19, 19);
        painter->setBrush(Qt::green);
        painter->drawRect(-40, -20*i-40, 19, 19);
        painter->setBrush(Qt::blue);
        painter->drawRect(-20*i-40, -40, 19, 19);
        painter->setBrush(Qt::yellow);
        painter->drawRect(-820, -20*i-40, 19, 19);
    }
}

SnakeWall::WallSettings SnakeWall::fnMatchWall(SnakeBlock *pHead,KeyPressEventFilter::Direction headDir) //Power-Up for changing the wall facing the snake head to the snake head color
{
    //----- [Setup] -----
    int iHeadColor = pHead->fnGetColor();
    SnakeWall::WallSettings settings;

    //----- [Control Path for head direction and color] -----
    switch(headDir)
    {
        case KeyPressEventFilter::North:
        switch(iHeadColor)
        {
            case 0: //Red Top
            settings.iXPos = 820;
            settings.iYPos = 820;
            settings.iRot = 0;
            iWallState = 0;
            break;
            case 1: //Green Top
            settings.iXPos = 820;
            settings.iYPos = -20;
            settings.iRot = 270;
            iWallState = 1;
            break;
            case 2: //Blue Top
            settings.iXPos = -20;
            settings.iYPos = -20;
            settings.iRot = 180;
            iWallState = 2;
            break;
            case 3: //Yellow Top
            settings.iXPos = -20;
            settings.iYPos = 820;
            settings.iRot = 90;
            iWallState = 3;
            break;
        }
        break;
        case KeyPressEventFilter::East:
        switch(iHeadColor)
        {
            case 0: //Red Right
            settings.iXPos = -20;
            settings.iYPos = 820;
            settings.iRot = 90;
            iWallState = 3;
            break;
            case 1: //Green Right
            settings.iXPos = 820;
            settings.iYPos = 820;
            settings.iRot = 0;
            break;
            case 2: //Blue Right
            settings.iXPos = 820;
            settings.iYPos = -20;
            settings.iRot = 270;
            iWallState = 1;
            break;
            case 3: //Yellow Right
            settings.iXPos = -20;
            settings.iYPos = -20;
            settings.iRot = 180;
            iWallState = 2;
            break;
        }
        break;
        case KeyPressEventFilter::South:
        switch(iHeadColor)
        {
            case 0: //Red Down
            settings.iXPos = -20;
            settings.iYPos = -20;
            settings.iRot = 180;
            iWallState = 2;
            break;
            case 1: //Green Down
            settings.iXPos = -20;
            settings.iYPos = 820;
            settings.iRot = 90;
            iWallState = 3;
            break;
            case 2: //Blue Down
            settings.iXPos = 820;
            settings.iYPos = 820;
            settings.iRot = 0;
            iWallState = 0;
            break;
            case 3: //Yellow Down
            settings.iXPos = 820;
            settings.iYPos = -20;
            settings.iRot = 270;
            iWallState = 1;
            break;
        }
        break;
        case KeyPressEventFilter::West:
        switch(iHeadColor)
        {
            case 0: //Red Left
            settings.iXPos = 820;
            settings.iYPos = -20;
            settings.iRot = 270;
            iWallState = 1;
            break;
            case 1: //Green Left
            settings.iXPos = -20;
            settings.iYPos = -20;
            settings.iRot = 180;
            iWallState = 2;
            break;
            case 2: //Blue Left
            settings.iXPos = -20;
            settings.iYPos = 820;
            settings.iRot = 90;
            iWallState = 3;
            break;
            case 3: //Yellow Left
            settings.iXPos = 820;
            settings.iYPos = 820;
            settings.iRot = 0;
            iWallState = 0;
            break;
        }
        break;
    }
    return settings;
}

int SnakeWall::fnGetWallState()
{
    return iWallState;
}

int SnakeWall::fnGetFacingColor(KeyPressEventFilter::Direction headDir) //Power-up for changing snake head color to wall it is facing
{
    //----- [Control path for head direction & wall state] -----
    switch(headDir)
    {
        case KeyPressEventFilter::North: //Here the wall state and head color matchup so expression is simplfied
        return iWallState;
        break;
        case KeyPressEventFilter::East:
        switch(iWallState)
        {
            case 0:
            return 1;
            break;
            case 1:
            return 2;
            break;
            case 2:
            return 3;
            break;
            case 3:
            return 0;
            break;
        }
        break;
        case KeyPressEventFilter::South:
        switch(iWallState)
        {
            case 0:
            return 2;
            break;
            case 1:
            return 3;
            break;
            case 2:
            return 0;
            break;
            case 3:
            return 1;
            break;
        }
        break;
        case KeyPressEventFilter::West:
        switch(iWallState)
        {
            case 0:
            return 3;
            break;
            case 1:
            return 0;
            break;
            case 2:
            return 1;
            break;
            case 3:
            return 2;
            break;
        }
        break;
    }
}

