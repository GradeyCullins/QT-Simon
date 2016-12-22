#ifndef SIMON_H
#define SIMON_H

#include "mainwindow.h"
#include <QObject>
#include <stdlib.h>
#include <time.h>

class MainWindow;

class Simon : public QObject
{
    Q_OBJECT

public:
    Simon(MainWindow*);

public slots:
    void onStartGame();
    void onNextRound();
    void onSimonButtonClicked(int);
    void onRecordInput();

signals:
    void nextRound();
    void finishedPlaying();
    void flashColors(std::vector<int>);
    void increaseRoundSpeed();

private:
    MainWindow *mainWindow;
    std::vector<int> roundMoves;
    std::vector<int> playerMoves;
    int currentButton;
    bool inputActive; // if the game is accepting and recording user input
    bool playerTurn; // true = player's turn

    void isMoveCorrect(int);
    void resetData();
};

#endif // SIMON_H
