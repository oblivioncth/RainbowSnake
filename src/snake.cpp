#include "snake.h"

Snake::Snake() //Linked-list root class
{
    SnakeBlock *main = new SnakeBlock(true);
    pHead = main;
    pCurs = main;
}

void Snake::fnAddBlock() //Connects picked up block to snake
{
    pFree->nextBlock = pHead;
    fnSetHead(pFree);
}

void Snake::fnNewFreeBlock() //Makes a new free block for linked-list
{
    SnakeBlock *free = new SnakeBlock(false);
    pFree = free;
}

Snake::BlockPtr Snake::fnGetHead()
{
    return pHead;
}

Snake::BlockPtr Snake::fnGetFree()
{
    return pFree;
}

Snake::BlockPtr Snake::fnGetCurs()
{
    return pCurs;
}

void Snake::fnSetHead(BlockPtr ptr)
{
    pHead = ptr;
    pCurs = pHead; //Keeps cursor at front until move or collision checks
}

void Snake::fnAdvCurs()
{
    pCurs = pCurs->nextBlock;
}

void Snake::fnResetCurs()
{
    pCurs = pHead;
}

bool Snake::fnCheckCollide(int inX, int inY, int iMode) //Checks collision with snake. Mode 1: Head, Mode 2: Body, Mode 3: Head & Body
{
    if(iMode < 1 || iMode > 3) //Checks for valid mode
        throw std::invalid_argument( "Invalid mode specified" );
    if(iMode == 2) //Sets to ignore head
        fnAdvCurs();
    while(pCurs != NULL)
    {
        if(inX == pCurs->pos().x() && inY == pCurs->pos().y())
        {
            if(iMode == 2 && pHead->fnGetColor() == pCurs->fnGetColor()) //Allows snake to pass through itself if block colors are the same, safe to do here because Mode 2 is only used for self crash check
                return false;
            else
                return true;
        }
        fnAdvCurs();
        if(iMode == 1) //End after one loop if checking only head
        {
            break;
        }
    }
    fnResetCurs();
    return false;
}
