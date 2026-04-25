#include "baldawindow.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QAbstractItemView>

BaldaWindow::BaldaWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    board = new BaldaBoard(this);

    startBtn = new QPushButton("Start");
    pauseBtn = new QPushButton("Pause");
    applyBtn = new QPushButton("Apply");

    timeLabel = new QLabel("02:00");
    letterLabel = new QLabel("Буква: —");

    playerWords = new QListWidget;
    computerWords = new QListWidget;

    boardOpacityEffect = new QGraphicsOpacityEffect(this);
    boardOpacityEffect->setOpacity(1.0);
    board->setGraphicsEffect(boardOpacityEffect);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *topLayout = new QHBoxLayout;
    QHBoxLayout *centerLayout = new QHBoxLayout;
    QVBoxLayout *rightLayout = new QVBoxLayout;
    QFrame *topCard = new QFrame;
    QFrame *rightCard = new QFrame;
    QHBoxLayout *topCardLayout = new QHBoxLayout(topCard);
    QVBoxLayout *rightCardLayout = new QVBoxLayout(rightCard);

    setWindowTitle("Balda");
    setMinimumSize(920, 680);
    mainLayout->setContentsMargins(18, 18, 18, 18);
    mainLayout->setSpacing(14);

    topCardLayout->setContentsMargins(16, 14, 16, 14);
    topCardLayout->setSpacing(10);
    topCardLayout->addWidget(startBtn);
    topCardLayout->addWidget(pauseBtn);
    topCardLayout->addWidget(applyBtn);
    topCardLayout->addStretch();
    topCardLayout->addWidget(new QLabel("Время:"));
    topCardLayout->addWidget(timeLabel);
    topLayout->addWidget(topCard);

    rightCardLayout->setContentsMargins(16, 16, 16, 16);
    rightCardLayout->setSpacing(10);
    rightCardLayout->addWidget(new QLabel("Игрок"));
    rightCardLayout->addWidget(playerWords);
    rightCardLayout->addWidget(new QLabel("Компьютер"));
    rightCardLayout->addWidget(computerWords);
    rightCardLayout->addWidget(letterLabel);
    rightLayout->addWidget(rightCard);

    centerLayout->addWidget(board, 1);
    centerLayout->addLayout(rightLayout);

    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(centerLayout);

    createLetterButtons(mainLayout);

    central->setLayout(mainLayout);
    applyModernStyle();

    auto addShadow = [](QWidget *w, int blur, int yOffset) {
        QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(w);
        shadow->setBlurRadius(blur);
        shadow->setOffset(0, yOffset);
        shadow->setColor(QColor(0, 0, 0, 70));
        w->setGraphicsEffect(shadow);
    };

    addShadow(topCard, 22, 2);
    addShadow(rightCard, 22, 2);

    connect(startBtn, SIGNAL(clicked()),
            this, SLOT(onStartClicked()));

    connect(pauseBtn, SIGNAL(clicked()),
            board, SLOT(pause()));

    connect(applyBtn, SIGNAL(clicked()),
            board, SLOT(apply()));

    connect(board, SIGNAL(timeChanged(QString)),
            timeLabel, SLOT(setText(QString)));

    connect(board, &BaldaBoard::pl_addWord,
            this, [this](const QString &word){
                playerWords->addItem(word);
            });

    connect(board, &BaldaBoard::comp_addWord,
            this, [this](const QString &word){
                computerWords->addItem(word);
            });
}

void BaldaWindow::createLetterButtons(QVBoxLayout *parentLayout)
{
    letterGroup = new QButtonGroup(this);
    letterGroup->setExclusive(true);

    QGridLayout *grid = new QGridLayout;
    QString letters = "АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";

    int row = 0;
    int col = 0;

    for (int i = 0; i < letters.size(); ++i) {
        QPushButton *btn = new QPushButton(QString(letters[i]));
        btn->setCheckable(true);
        btn->setMinimumSize(38, 38);

        letterGroup->addButton(btn);
        grid->addWidget(btn, row, col);

        col++;
        if (col == 8) {
            col = 0;
            row++;
        }
    }

    parentLayout->addLayout(grid);

    connect(letterGroup,
            SIGNAL(buttonClicked(QAbstractButton*)),
            this,
            SLOT(onLetterClicked(QAbstractButton*)));
}

void BaldaWindow::applyModernStyle()
{
    setStyleSheet(
        "QMainWindow { background-color: #f1f5f9; }"
        "QFrame { background: #ffffff; border-radius: 14px; }"
        "QPushButton {"
        "  background-color: #ffffff;"
        "  border: 1px solid #d5dde8;"
        "  border-radius: 10px;"
        "  padding: 8px 14px;"
        "  color: #1f2937;"
        "  font-size: 12pt;"
        "  font-weight: 600;"
        "}"
        "QPushButton:hover { background-color: #eef2ff; border-color: #b8c3ff; }"
        "QPushButton:pressed { background-color: #dbe4ff; }"
        "QPushButton:checked { background-color: #4f46e5; color: white; border-color: #4f46e5; }"
        "QLabel { color: #1e293b; font-size: 11pt; font-weight: 600; }"
        "QListWidget {"
        "  background-color: #fbfdff;"
        "  border: 1px solid #d8e1ed;"
        "  border-radius: 10px;"
        "  padding: 6px;"
        "  outline: none;"
        "}"
        "QListWidget::item { padding: 5px 6px; border-radius: 7px; }"
        "QListWidget::item:selected { background-color: #e8edff; color: #1d2a62; }"
    );

    playerWords->setSelectionMode(QAbstractItemView::NoSelection);
    computerWords->setSelectionMode(QAbstractItemView::NoSelection);
    timeLabel->setMinimumWidth(64);
}

void BaldaWindow::animateBoardEntrance()
{
    QPropertyAnimation *fade = new QPropertyAnimation(boardOpacityEffect, "opacity", this);
    fade->setDuration(220);
    fade->setStartValue(0.25);
    fade->setEndValue(1.0);
    fade->setEasingCurve(QEasingCurve::OutCubic);
    fade->start(QAbstractAnimation::DeleteWhenStopped);
}

void BaldaWindow::onLetterClicked(QAbstractButton *button)
{
    board->curletter(button->text());
    letterLabel->setText("Буква: " + button->text());
}
void BaldaWindow::onStartClicked()
{
    playerWords->clear();
    computerWords->clear();
    board->start();
    animateBoardEntrance();
}
