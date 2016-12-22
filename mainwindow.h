#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QTimer>
#include <QKeyEvent>
#include <simon.h>

using namespace std;

class Simon;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    Ui::MainWindow *ui; // TODO: this used to be private, may need to switch back
    ~MainWindow();

public slots:
    void onFinishedPlaying();
    void onFlashColors(std::vector<int>);
    void resetView();

protected:
    void keyPressEvent(QKeyEvent *);

private slots:
    // qt auto-generated slots
    void on_driverButton_clicked();
    void on_redButton_clicked();
    void on_blueButton_clicked();

    void onStartGame();
    void flashColor();
    void unflashColor();
    void onRecordInput();
    void onIncreaseRoundSpeed();

    void on_help_clicked();

signals:
    void startGame();
    void simonButtonClicked(int);
    void recordInput();

private:
    Simon *simon;
    std::vector<int> round;
    int currentColor;
    int roundSize; // keep track of number of colors in a round
    int colorsShown;
    int roundSpeed;
    bool isPlaying;
    bool enableHotkeys;

    void enableButtons();
    void disableButtons();   
    void updateProgressBar();
};

#endif // MAINWINDOW_H
