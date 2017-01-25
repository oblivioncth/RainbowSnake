#ifndef SNAKE_H
#define SNAKE_H

#include "snakeblock.h"

class Snake
{

private:
    typedef SnakeBlock* BlockPtr;
    BlockPtr pHead;
    BlockPtr pCurs;
    BlockPtr pFree;
public:
    Snake();
    ~Snake();
    void fnAddBlock();
    void fnNewFreeBlock();
    BlockPtr fnGetHead();
    BlockPtr fnGetFree();
    BlockPtr fnGetCurs();
    void fnSetHead(BlockPtr ptr);
    void fnAdvCurs();
    void fnResetCurs();
    bool fnCheckCollide(int inX, int inY, int iMode);
};

#endif // SNAKE_H
