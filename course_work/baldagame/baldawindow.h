#ifndef BALDAWINDOW_H
#define BALDAWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QGraphicsOpacityEffect>

#include "baldaboard.h"

class BaldaWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BaldaWindow(QWidget *parent = nullptr);

private slots:
    void onLetterClicked(QAbstractButton *button);
    void onStartClicked();


private:
    BaldaBoard *board;

    QPushButton *startBtn;
    QPushButton *pauseBtn;
    QPushButton *applyBtn;

    QLabel *timeLabel;
    QLabel *letterLabel;

    QListWidget *playerWords;
    QListWidget *computerWords;

    QButtonGroup *letterGroup;
    QGraphicsOpacityEffect *boardOpacityEffect;

    void createLetterButtons(QVBoxLayout *parentLayout);
    void applyModernStyle();
    void animateBoardEntrance();
};

#endif
