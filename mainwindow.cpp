#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->roundCount->setDecMode();
    ui->roundCount->setDigitCount(3);
    ui->roundCount->display(1);
    ui->progressBar->setValue(0);
    ui->helpMenu->setVisible(false);
    simon = new Simon(this);
    roundSize = 0;
    colorsShown = 0;
    isPlaying = false;
    enableHotkeys = true;
    roundSpeed = 1000;



    QObject::connect(this, SIGNAL(startGame()), this, SLOT(onStartGame()));
    QObject::connect(this, SIGNAL(startGame()), simon, SLOT(onStartGame()));
    QObject::connect(this, SIGNAL(simonButtonClicked(int)), simon, SLOT(onSimonButtonClicked(int)));
    QObject::connect(this, SIGNAL(recordInput()), simon, SLOT(onRecordInput()));
    QObject::connect(this, SIGNAL(recordInput()), this, SLOT(onRecordInput()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete simon;
}

//------------------------------------------------------------------------------------
//                      SLOT METHODS
//------------------------------------------------------------------------------------

void MainWindow::on_driverButton_clicked()
{
    emit MainWindow::startGame();
}

void MainWindow::on_redButton_clicked()
{
    if (isPlaying) updateProgressBar();
    emit MainWindow::simonButtonClicked(0);

}

void MainWindow::on_blueButton_clicked()
{
    if (isPlaying) updateProgressBar();
    emit MainWindow::simonButtonClicked(1);

}

/*
 * Handles view-related setup actions on a start game signal.
 */
void MainWindow::onStartGame()
{
    roundSpeed = 1000;
    ui->roundCount->display(1);

    // reset view
    MainWindow::resetView();
}

void MainWindow::resetView()
{
    ui->driverButton->setText("Restart");
    ui->label->setText("WAIT");
    ui->progressBar->setValue(0);
    ui->redButton->setStyleSheet("QPushButton {background-color:rgb(172, 42, 42);}QPushButton:hover {background-color:rgb(254, 5, 5);}");
    ui->blueButton->setStyleSheet("QPushButton {background-color:rgb(60, 107, 202)} QPushButton:hover {background-color:rgb(22, 60, 243)}");
    disableButtons();
}

void MainWindow::onFlashColors(std::vector<int> colors)
{
    ui->help->setEnabled(false);
    round = colors;
    roundSize = colors.size();
    currentColor = colors[0];
    QTimer::singleShot(1000, this, SLOT(flashColor()));
}

void MainWindow::flashColor()
{
    (currentColor == 0) ? ui->redButton->setStyleSheet("background-color:rgb(255, 0, 0)")
                         : ui->blueButton->setStyleSheet("background-color:rgb(0, 0, 255)");

    updateProgressBar();

    colorsShown++;

    QTimer::singleShot(roundSpeed, this, SLOT(unflashColor()));
}

void MainWindow::unflashColor()
{
    (currentColor == 0) ? ui->redButton->setStyleSheet("QPushButton {background-color:rgb(172, 42, 42);}QPushButton:hover {background-color:rgb(254, 5, 5);}")
                         : ui->blueButton->setStyleSheet("QPushButton {background-color:rgb(60, 107, 202)} QPushButton:hover {background-color:rgb(22, 60, 243)}");

    if (colorsShown != roundSize) // if there are more colors to display
    {
        currentColor = round[colorsShown];
        QTimer::singleShot(roundSpeed, this, SLOT(flashColor()));
    }
    else
    {
        currentColor = 0;
        colorsShown = 0;
        MainWindow::resetView();
        enableButtons();
        ui->help->setEnabled(true);

        // signal to the model that user input is to be recorded
        emit recordInput();
    }
}

void MainWindow::onRecordInput()
{
    isPlaying = true;
}

void MainWindow::onIncreaseRoundSpeed()
{
    (roundSpeed == 200) ? roundSpeed = 200 : roundSpeed -= 100;
    ui->roundCount->display(ui->roundCount->intValue() + 1);
}

void MainWindow::onFinishedPlaying()
{
    isPlaying = false;
    ui->label->setText("YOU LOST");
    ui->driverButton->setText("Restart");
    ui->roundCount->display(1);
    ui->redButton->setDisabled(true);
    ui->blueButton->setDisabled(true);
}

void MainWindow::keyPressEvent(QKeyEvent * e)
{
    if (enableHotkeys)
    {
        if (e->key() == Qt::Key_A)
            on_redButton_clicked();
        if (e->key() == Qt::Key_S)
            on_blueButton_clicked();
        if (e->key() == Qt::Key_Space)
            on_driverButton_clicked();
    }
}

//------------------------------------------------------------------------------------
//                      GAME METHODS
//------------------------------------------------------------------------------------

void MainWindow::enableButtons()
{
    ui->progressBar->setValue(0);
    ui->redButton->setDisabled(false);
    ui->blueButton->setDisabled(false);
    ui->driverButton->setDisabled(false);
    enableHotkeys = true;
    ui->label->setText("GO");
}

void MainWindow::disableButtons()
{
    ui->redButton->setDisabled(true);
    ui->blueButton->setDisabled(true);
    ui->driverButton->setDisabled(true);
    enableHotkeys = false;
}

void MainWindow::updateProgressBar()
{
    ui->progressBar->setValue(ui->progressBar->value() + 100 / roundSize);

    if (ui->progressBar->value() >= 95)
        ui->progressBar->setValue(100);
}

void MainWindow::on_help_clicked()
{
    (ui->helpMenu->isVisible()) ? ui->helpMenu->setVisible(false) : ui->helpMenu->setVisible(true);
    if (ui->helpMenu->isVisible()) disableButtons();
    else
    {
        ui->redButton->setDisabled(false);
        ui->blueButton->setDisabled(false);
        ui->driverButton->setDisabled(false);
    }
}
