#ifndef BALDABOARD_H
#define BALDABOARD_H

#include <QFrame>
#include <QButtonGroup>
#include <QPushButton>
#include <QTimer>
#include <QTime>
#include <QVector>
#include <QSet>

class BaldaBoard : public QFrame
{
    Q_OBJECT

public:
    explicit BaldaBoard(QWidget *parent = nullptr);
    void curletter(const QString &l);

signals:
    void timeChanged(const QString &);
    void pl_addWord(const QString &);
    void comp_addWord(const QString &);


public slots:
    void start();
    void pause();
    void apply();

private slots:
    void onBoardClicked(int);
    void timeOutSlot();

private:
    void placeStartWord();
    void loadDictionary();
    bool checkWord(const QString &word);
    bool isOneEditDerivative(const QString &candidate, const QString &baseWord) const;
    bool isArtificialDerivative(const QString &word) const;
    void computerMove();

private:
    QButtonGroup *board;
    QVector<QPushButton*> player;

    QString letter;
    QString playerWord;

    int curPos;
    const int rowSize = 5;
    const int colSize = 5;

    bool addLetter;
    bool isStarted;
    bool isPlayerTurn;

    QTimer timer;
    QTime time;

    QSet<QString> dictionary;
    QSet<QString> dictionaryPrefixes;
    QSet<QString> usedWords;
    int maxWordLength;
};

#endif
