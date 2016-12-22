#include "simon.h"

Simon::Simon(MainWindow* _mainWindow)
{
    mainWindow = _mainWindow;
    currentButton = 0;
    srand(time(NULL));

    QObject::connect(this, SIGNAL(flashColors(std::vector<int>)), mainWindow, SLOT(onFlashColors(std::vector<int>)));
    QObject::connect(this, SIGNAL(finishedPlaying()), mainWindow, SLOT(onFinishedPlaying()));
    QObject::connect(this, SIGNAL(nextRound()), this, SLOT(onNextRound()));
    QObject::connect(this, SIGNAL(increaseRoundSpeed()), mainWindow, SLOT(onIncreaseRoundSpeed()));
}

//------------------------------------------------------------------------------------
//                      SLOT METHODS
//------------------------------------------------------------------------------------

/*
 * Handles data-specific setup for a new Simon game
 */
void Simon::onStartGame()
{
    // reset data
    resetData();

    // generate round
    onNextRound();
}

void Simon::onNextRound()
{
    int nextColor = rand() % 2;

    roundMoves.push_back(nextColor);

    if (roundMoves.size() < 2)
    {
        onNextRound();
    }
    else
    {
        // display color round on screen
        if(!inputActive)
            emit flashColors(roundMoves);
    }
}

void Simon::onSimonButtonClicked(int button)
{
    if (inputActive)
    {
        playerMoves.push_back(button);
        isMoveCorrect(currentButton);
    }
}

void Simon::onRecordInput()
{
    inputActive = true;
}

//------------------------------------------------------------------------------------
//                      GAME METHODS
//------------------------------------------------------------------------------------

void Simon::isMoveCorrect(int index)
{
    if (playerMoves[index] != roundMoves[index])
    {
        resetData();
        emit finishedPlaying();
    }
    else
    {
        if (index == (int)roundMoves.size() - 1)
        {
            // slight delay before the view is reset for better UX
            QTimer::singleShot(1000, mainWindow, SLOT(resetView()));

            // reset user model data for next round
            inputActive = false;
            currentButton = 0;
            playerMoves.clear();
            playerMoves.resize(0);

            // send signals to notify View of new round
            emit increaseRoundSpeed();
            emit nextRound();
        }
        else
        {
            currentButton++;
        }
    }
}

void Simon::resetData()
{
    inputActive = false;
    roundMoves.clear();
    roundMoves.resize(0);
    playerMoves.clear();
    playerMoves.resize(0);
    currentButton = 0;
}
