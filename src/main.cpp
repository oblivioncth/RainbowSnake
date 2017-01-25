#include <QtWidgets>
#include <QSound>

#include <stack>

#include "snakeblock.h"
#include "keypresseventfilter.h"
#include "snake.h"
#include "snakewall.h"

//----- [Global instance access as required] -----
Snake *MainSnake = new Snake;
KeyPressEventFilter *filterPointer; //Allows key press access in update function
QGraphicsScene *scenePointer;
QLineEdit *scorePointer;
QLineEdit *highscorePointer;
QLineEdit *powerPointer;
std::stack <int> powerStack;

//----- [Setup color walls] -----
SnakeWall *snakeWall = new SnakeWall;

//----- [Update function gamestate flags] -----
bool bFirstPassFlag = true;
bool bFreeBlock = false;

//----- [Scores] -----
int iScore = 0;
int iHighScore;

//----- [Counters] -----
int iPowerCounter = 0;
int iTimeStretchCounter = 0;

//----- [Game speed] -----
QTimer timer;
int iTimerSpeed = 70;

//----- [Function prototypes] -----
void slotUpdate();
QPoint fnCrashAndLoopCheck(int inX, int inY);
void fnGameOver();
void fnUpdatePowerText();


int main(int argc, char **argv)
{
    //----- [Qt App & key grab setup] -----
    QApplication app(argc, argv);
    KeyPressEventFilter *filter = new KeyPressEventFilter(&app);
    filterPointer = filter;
    app.installEventFilter(filter);
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    //----- [Graphics Scene settings] -----
    QGraphicsScene scene;
    scenePointer = &scene;
    scene.setSceneRect(0, 0, 800, 800);
    scene.setItemIndexMethod(QGraphicsScene::NoIndex);

    //----- [Initialize Scene & snake head] -----
    MainSnake->fnGetHead()->setPos(400,400);
    scene.addItem(MainSnake->fnGetHead());

    //----- [Initialize Snake Wall] -----
    snakeWall->setPos(820,820);
    scene.addItem(snakeWall);

    //----- [Graphics View settings] -----
    QGraphicsView view(&scene);
    view.setBackgroundBrush(QPixmap(":/images/Gamespace.png"));
    view.setCacheMode(QGraphicsView::CacheBackground);
    view.setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view.setDragMode(QGraphicsView::ScrollHandDrag);
    view.resize(802, 802);

    //----- [UI Setup: Layouts & Labels] -----
    QWidget widget;
    QHBoxLayout hLayout;
    QVBoxLayout vLayout;
    QGroupBox scoreGroupBox;
    QGroupBox highscoreGroupBox;
    QGroupBox powerGroupBox;
    scoreGroupBox.setTitle("SCORE");
    scoreGroupBox.setAlignment(Qt::AlignCenter);
    highscoreGroupBox.setTitle("HIGHSCORE");
    highscoreGroupBox.setAlignment(Qt::AlignCenter);
    powerGroupBox.setTitle("CURRENT POWER-UP");
    powerGroupBox.setAlignment(Qt::AlignCenter);

    //----- [UI Setup: Fields] -----
    QLineEdit scoreLine;
    QLineEdit highscoreLine;
    QLineEdit powerLine;
    scoreLine.setText(QString::number(iScore));
    scoreLine.setAlignment(Qt::AlignRight);
    scorePointer = &scoreLine;
    highscoreLine.setAlignment(Qt::AlignRight);
    highscorePointer = &highscoreLine;
    powerLine.setAlignment(Qt::AlignCenter);
    powerLine.setText("NONE");
    powerPointer = &powerLine;


    //----- [UI Setup: Finalize] -----
    QVBoxLayout scoreVBox;
    QVBoxLayout highscoreVBox;
    QVBoxLayout powerVBox;
    scoreVBox.addWidget(&scoreLine);
    scoreGroupBox.setLayout(&scoreVBox);
    highscoreVBox.addWidget(&highscoreLine);
    highscoreGroupBox.setLayout(&highscoreVBox);
    powerVBox.addWidget(&powerLine);
    powerGroupBox.setLayout(&powerVBox);
    widget.setLayout(&hLayout);
    hLayout.addWidget(&view);
    hLayout.addLayout(&vLayout);
    vLayout.addWidget(&scoreGroupBox);
    vLayout.addWidget(&highscoreGroupBox);
    vLayout.addWidget(&powerGroupBox);
    vLayout.setAlignment(Qt::AlignTop);

    //----- [Get Highscore from file] -----
    QFile scoreFile("highscore");
    if (scoreFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream scoreIn(&scoreFile);
        QString sHighScore = scoreIn.readLine();
        iHighScore = sHighScore.toInt();
        highscorePointer->setText(QString::number(iHighScore));
        scoreFile.close();
    }
    else
    {
        iHighScore = 0; //Default
        highscorePointer->setText(QString::number(iHighScore));
    }

    //----- [Splash setup] -----
    QPixmap qpmSplash(":/images/SnakeSplash.png");
    QSplashScreen qssSplash(qpmSplash);
    qssSplash.show();
    QThread::sleep(1.5);
    qssSplash.finish(&widget);

    //----- [Main Window Settings] -----
    widget.setFixedSize(1000,826);
    widget.setWindowTitle("Rainbow Snake");

    //----- [Center & show window] -----
    QRect screen = QApplication::desktop()->screenGeometry();
    int screenHeight = screen.height();
    int screenWidth = screen.width();
    widget.move(screenWidth/2 - 500, screenHeight/2 - 413);
    widget.show();


    //----- [Setup & start game clock] -----
    QObject::connect(&timer, &QTimer::timeout, slotUpdate);
    timer.start(iTimerSpeed);

    return app.exec(); //Thread loop call
}

void slotUpdate() //Game update function for clock
{
    //----- [Get most recent keypress from buffer] -----
    filterPointer->fnReleaseDirBuffer();

    //----- [MAIN GAME LOOP] -----
    if(filterPointer->bGameRunning == true) //Keeps game from starting until player selects first direction
    {
        //----- [Check block pickup] -----
        if(!bFirstPassFlag) //Can't check collision with block that doesn't exist yet, so only run after 1st block generation
        {
            SnakeBlock *Free = MainSnake->fnGetFree();
            if(MainSnake->fnCheckCollide(Free->pos().x(),Free->pos().y(),1)) //Checks if snake head is touching free block
            {
                //----- [Add block to snake and make it the head of the linked list] -----
                MainSnake->fnAddBlock();
                bFreeBlock = false;
                iScore += 100;
                iPowerCounter ++; //Increment for tracking when to give new power-up
                scorePointer->setText(QString::number(iScore));
                QSound::play(":/sound/pickup.wav"); //Play pickup sound
            }
        }
        bFirstPassFlag = false;

        //----- [Generate new block] -----
        if(!bFreeBlock)
        {
            //----- [Generate spawn location] -----
            int iNewX;
            int iNewY;
            do
            {
                iNewX = qrand() % 38 + 1;
                iNewY = qrand() % 38 + 1;
            } while(MainSnake->fnCheckCollide(iNewX,iNewY,3)); //Guarantees block won't spawn inside of Snake

            //----- [Set location & and block] -----
            MainSnake->fnNewFreeBlock();
            MainSnake->fnGetFree()->setPos(iNewX*20+20,iNewY*20+20);
            scenePointer->addItem(MainSnake->fnGetFree());
            bFreeBlock = true;
        }

        //----- [Manage power-ups] -----
        if(iPowerCounter == 10 && powerStack.size() < 3) //Checks statck and power up gift timing
        {
            int iNewPower = qrand() % 3;
            powerStack.push(iNewPower);
            iPowerCounter = 0;
            fnUpdatePowerText();
        }
        if(filterPointer->bUsingPower) //When power is used
        {
            if(!powerStack.empty())
            {
                switch(powerStack.top())
                {
                    case 0: //Power: Match Wall
                    SnakeWall::WallSettings newSettings = snakeWall->fnMatchWall(MainSnake->fnGetHead(),filterPointer->getDirection());
                    snakeWall->setPos(newSettings.iXPos,newSettings.iYPos);
                    snakeWall->setRotation(newSettings.iRot);
                    powerStack.pop();
                    break;

                    case 1: //Power: Match Head
                    MainSnake->fnGetHead()->fnSetColor(snakeWall->fnGetFacingColor(filterPointer->getDirection()));
                    powerStack.pop();
                    break;

                    case 2: //Power: Time Stretch
                    timer.setInterval(280);
                    iTimeStretchCounter = 20;
                    powerStack.pop();
                    break;
                }
            }
            filterPointer->bUsingPower = false;
            fnUpdatePowerText(); //Refresh UI Text
        }

        //----- [Move Snake] -----
        KeyPressEventFilter::Direction SnakeDir = filterPointer->getDirection();
        int iCurrentX;
        int iCurrentY;
        int iPrevX;
        int iPrevY;
        QPoint pntNext;
        while(MainSnake->fnGetCurs() != NULL) //Linked list loop for all snake block pieces
        {
            SnakeBlock *CurrentBlock = MainSnake->fnGetCurs();
            iCurrentX = CurrentBlock->pos().x();
            iCurrentY = CurrentBlock->pos().y();
            if(CurrentBlock == MainSnake->fnGetHead())
            {
                switch(SnakeDir) //Snake movement logic
                {
                    case KeyPressEventFilter::North:
                        pntNext = fnCrashAndLoopCheck(iCurrentX,iCurrentY-20);
                        CurrentBlock->setPos(pntNext.x(),pntNext.y());
                    break;
                    case KeyPressEventFilter::East:
                        pntNext = fnCrashAndLoopCheck(iCurrentX+20,iCurrentY);
                        CurrentBlock->setPos(pntNext.x(),pntNext.y());
                    break;
                    case KeyPressEventFilter::South:
                        pntNext = fnCrashAndLoopCheck(iCurrentX,iCurrentY+20);
                        CurrentBlock->setPos(pntNext.x(),pntNext.y());
                    break;
                    case KeyPressEventFilter::West:
                        pntNext = fnCrashAndLoopCheck(iCurrentX-20,iCurrentY);
                        CurrentBlock->setPos(pntNext.x(),pntNext.y());
                    break;
                }
            }
            else
            {
                CurrentBlock->setPos(iPrevX,iPrevY);
            }
            iPrevX = iCurrentX;
            iPrevY = iCurrentY;
            MainSnake->fnAdvCurs();
        }
        MainSnake->fnResetCurs(); //Cursor always defaults to head when a check is not being performed

        //----- [Reset game speed when time stretch expires] -----
        if(iTimeStretchCounter != 0)
        {
            iTimeStretchCounter--;
            if(iTimeStretchCounter == 0)
                timer.setInterval(70);
        }

        //----- [Redraw walls for conflict removal] -----
        snakeWall->setPos(snakeWall->pos().x(),snakeWall->pos().y());
    }
}

QPoint fnCrashAndLoopCheck(int inX, int inY) //Checks for game over and same color looping conditions
{
    QPoint ptnNextRtn;
    int iHeadColor = MainSnake->fnGetHead()->fnGetColor();
    int iWS = snakeWall->fnGetWallState();

    //----- [Self-hit check] -----
    if(MainSnake->fnCheckCollide(inX,inY,2))
        fnGameOver();

    //----- [Wall hit check] -----
    if(inY == 20) //Hitting top wall
    {
        if(iHeadColor == iWS) //Here the WallState and HeadColor values line up so statement is simplified
        {
            ptnNextRtn.setX(inX);
            ptnNextRtn.setY(780);
            return ptnNextRtn;
        }
        else
            fnGameOver();
    }
    if(inX == 800) //Hitting right wall
    {
        if((iHeadColor == 0 && iWS == 3) || (iHeadColor == 1 && iWS == 0) || (iHeadColor == 2 && iWS == 1) || (iHeadColor == 3 && iWS == 2))
        {
            ptnNextRtn.setX(40);
            ptnNextRtn.setY(inY);
            return ptnNextRtn;
        }
        else
            fnGameOver();
    }
    if(inY == 800) //Hitting bottom wall
    {
        if((iHeadColor == 0 && iWS == 2) || (iHeadColor == 1 && iWS == 3) || (iHeadColor == 2 && iWS == 0) || (iHeadColor == 3 && iWS == 1))
        {
            ptnNextRtn.setX(inX);
            ptnNextRtn.setY(40);
            return ptnNextRtn;
        }
        else
            fnGameOver();
    }
    if(inX == 20) //Hitting left wall
    {
        if((iHeadColor == 0 && iWS == 1) || (iHeadColor == 1 && iWS == 2) || (iHeadColor == 2 && iWS == 3) || (iHeadColor == 3 && iWS == 0))
        {
            ptnNextRtn.setX(800);
            ptnNextRtn.setY(inY);
            return ptnNextRtn;
        }
        else
            fnGameOver();
    }

    //----- [Default if no collision] -----
    ptnNextRtn.setX(inX);
    ptnNextRtn.setY(inY);
    return ptnNextRtn;
}

void fnGameOver() //Stops game and triggers "Game Over" message
{
    //----- [Game over message box] -----
    QMessageBox endScreen;
    endScreen.setWindowTitle("GAME OVER");
    endScreen.setText("GAME OVER!");
    endScreen.setInformativeText("You scored " + QString::number(iScore) + " points.");
    endScreen.exec();

    //----- [Save score if best] -----
    if(iScore > iHighScore)
    {
        QFile scoreFile("highscore");
        scoreFile.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream scoreOut(&scoreFile);
        scoreOut << QString::number(iScore);
        scoreFile.close();
    }

    //----- [Restart game] -----
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

void fnUpdatePowerText() //The current power-up text in UI
{
    if(powerStack.empty())
       powerPointer->setText("NONE");
    else
    {
        switch(powerStack.top())
        {
            case 0:
            powerPointer->setText("Match Wall");
            break;

            case 1:
            powerPointer->setText("Match Head");
            break;

            case 2:
            powerPointer->setText("Time Stretch");
            break;
        }
    }
}

