#ifndef SNAKEBLOCK_H
#define SNAKEBLOCK_H

#include <QGraphicsItem>

class SnakeBlock : public QGraphicsItem
{
public:
    SnakeBlock(bool bHead);

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) Q_DECL_OVERRIDE;
    SnakeBlock *nextBlock = NULL;
    int fnGetColor();
    void fnSetColor(int iNewColor);
protected:

private:
    int iColor;
};

#endif
