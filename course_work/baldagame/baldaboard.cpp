#include "baldaboard.h"
#include <QGridLayout>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QEasingCurve>
#include <QRandomGenerator>
#include <algorithm>
#include <functional>

namespace {
const QString kCellStyle =
    "QPushButton {"
    " background-color: #ffffff;"
    " border: 1px solid #d6deea;"
    " border-radius: 10px;"
    " color: #0f172a;"
    " font-size: 18pt;"
    " font-weight: 700;"
    "}";

const QString kSelectedCellStyle =
    "QPushButton {"
    " background-color: #eef2ff;"
    " border: 1px solid #7687ff;"
    " border-radius: 10px;"
    " color: #334155;"
    " font-size: 18pt;"
    " font-weight: 700;"
    "}";
}

BaldaBoard::BaldaBoard(QWidget *parent)
    : QFrame(parent),
    curPos(-1),
    addLetter(false),
    isStarted(false),
    isPlayerTurn(true),
    maxWordLength(0)
{
    setFrameStyle(QFrame::Panel | QFrame::Sunken);
    setStyleSheet("QFrame { background-color: transparent; border: 0; }");
    setEnabled(false);

    board = new QButtonGroup(this);
    board->setExclusive(false);

    QGridLayout *layout = new QGridLayout(this);
    layout->setSpacing(6);
    layout->setContentsMargins(4, 4, 4, 4);

    for (int i = 0; i < rowSize; ++i) {
        for (int j = 0; j < colSize; ++j) {
            QPushButton *btn = new QPushButton("");
            btn->setMinimumSize(50, 50);
            btn->setStyleSheet(kCellStyle);

            int id = i * colSize + j;
            board->addButton(btn, id);
            layout->addWidget(btn, i, j);
        }
    }

    connect(board, SIGNAL(buttonClicked(int)),
            this, SLOT(onBoardClicked(int)));
    connect(&timer, SIGNAL(timeout()),
            this, SLOT(timeOutSlot()));

    loadDictionary();
}

void BaldaBoard::loadDictionary()
{
    QFile file(":/dict/word_rus.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString word = in.readLine().trimmed().toUpper();
        if (!word.isEmpty()) {
            dictionary.insert(word);
            maxWordLength = std::max(maxWordLength, word.size());
            for (int i = 1; i <= word.size(); ++i)
                dictionaryPrefixes.insert(word.left(i));
        }
    }
}

void BaldaBoard::curletter(const QString &l)
{
    letter = l;
}

void BaldaBoard::start()
{
    isStarted = true;
    isPlayerTurn = true;
    addLetter = false;
    player.clear();
    usedWords.clear();
    curPos = -1;

    setEnabled(true);

    time = QTime(0, 2, 0);
    emit timeChanged(time.toString("mm:ss"));
    timer.start(1000);

    placeStartWord();
}

void BaldaBoard::placeStartWord()
{
    QString word = "БАЛДА";
    int row = rowSize / 2;

    for (int i = 0; i < word.size(); ++i)
        board->button(row * colSize + i)->setText(QString(word[i]));

    usedWords.insert(word);
}

bool BaldaBoard::checkWord(const QString &word)
{
    if (word.size() < 3)
        return false;

    if (!dictionary.contains(word))
        return false;

    if (usedWords.contains(word))
        return false;

    if (isArtificialDerivative(word))
        return false;

    return true;
}

bool BaldaBoard::isOneEditDerivative(const QString &candidate, const QString &baseWord) const
{
    if (candidate == baseWord)
        return false;

    const int candLen = candidate.size();
    const int baseLen = baseWord.size();
    if (std::abs(candLen - baseLen) > 1)
        return false;

    int i = 0;
    int j = 0;
    int edits = 0;

    while (i < candLen && j < baseLen) {
        if (candidate[i] == baseWord[j]) {
            ++i;
            ++j;
            continue;
        }

        ++edits;
        if (edits > 1)
            return false;

        if (candLen > baseLen) {
            ++i;
        } else if (candLen < baseLen) {
            ++j;
        } else {
            ++i;
            ++j;
        }
    }

    if (i < candLen || j < baseLen)
        ++edits;

    return edits == 1;
}

bool BaldaBoard::isArtificialDerivative(const QString &word) const
{
    for (const QString &used : usedWords) {
        if (isOneEditDerivative(word, used))
            return true;

        if ((word.startsWith(used) || word.endsWith(used)) && std::abs(word.size() - used.size()) <= 1)
            return true;
    }

    return false;
}

void BaldaBoard::apply()
{
    if (!isPlayerTurn || playerWord.isEmpty())
        return;

    if (!checkWord(playerWord)) {
        QMessageBox::warning(
            this,
            "Ошибка",
            "Слово \"" + playerWord + "\" не прошло проверку словаря или является производным"
            );

        return;
    }

    usedWords.insert(playerWord);
    emit pl_addWord(playerWord);

    player.clear();
    playerWord.clear();
    addLetter = false;
    curPos = -1;

    for (auto *b : board->buttons())
        b->setStyleSheet(kCellStyle);

    isPlayerTurn = false;
    computerMove();
}

void BaldaBoard::computerMove()
{
    struct CandidateMove {
        int cellId;
        QString word;
        QChar letter;
    };

    QVector<QString> boardLetters(rowSize * colSize);
    for (int id = 0; id < rowSize * colSize; ++id) {
        QPushButton *cell = qobject_cast<QPushButton*>(board->button(id));
        boardLetters[id] = cell ? cell->text() : QString();
    }

    auto neighborsOf = [this](int id) {
        QVector<int> neighbors;
        const int r = id / colSize;
        const int c = id % colSize;
        if (c > 0)
            neighbors.push_back(id - 1);
        if (c + 1 < colSize)
            neighbors.push_back(id + 1);
        if (r > 0)
            neighbors.push_back(id - colSize);
        if (r + 1 < rowSize)
            neighbors.push_back(id + colSize);
        return neighbors;
    };

    auto hasLetterNeighbor = [&](int id) {
        for (int n : neighborsOf(id)) {
            if (!boardLetters[n].isEmpty())
                return true;
        }
        return false;
    };

    QVector<CandidateMove> moves;
    const QString alphabet = "АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";

    for (int id = 0; id < rowSize * colSize; ++id) {
        if (!boardLetters[id].isEmpty())
            continue;
        if (!hasLetterNeighbor(id))
            continue;

        for (QChar inserted : alphabet) {
            boardLetters[id] = QString(inserted);

            QSet<QString> foundWords;
            QVector<bool> visited(rowSize * colSize, false);
            QString current;

            std::function<void(int, bool)> dfs = [&](int pos, bool includesInserted) {
                if (visited[pos] || boardLetters[pos].isEmpty())
                    return;

                visited[pos] = true;
                current += boardLetters[pos];

                if (current.size() <= maxWordLength && !dictionaryPrefixes.contains(current)) {
                    current.chop(1);
                    visited[pos] = false;
                    return;
                }

                const bool nowIncludesInserted = includesInserted || (pos == id);
                if (nowIncludesInserted && checkWord(current))
                    foundWords.insert(current);

                if (current.size() < maxWordLength) {
                    for (int n : neighborsOf(pos))
                        dfs(n, nowIncludesInserted);
                }

                current.chop(1);
                visited[pos] = false;
            };

            for (int start = 0; start < rowSize * colSize; ++start) {
                if (!boardLetters[start].isEmpty())
                    dfs(start, false);
            }

            for (const QString &word : foundWords)
                moves.push_back({id, word, inserted});

            boardLetters[id].clear();
        }
    }

    if (!moves.isEmpty()) {
        const int moveIndex = QRandomGenerator::global()->bounded(moves.size());
        const CandidateMove move = moves.at(moveIndex);
        QPushButton *targetCell = qobject_cast<QPushButton*>(board->button(move.cellId));
        if (targetCell) {
            targetCell->setText(QString(move.letter));
            targetCell->setStyleSheet(kSelectedCellStyle);

            usedWords.insert(move.word);
            emit comp_addWord(move.word);

            QRect original = targetCell->geometry();
            QRect grown = original.adjusted(-2, -2, 2, 2);
            QSequentialAnimationGroup *group = new QSequentialAnimationGroup(this);
            QPropertyAnimation *grow = new QPropertyAnimation(targetCell, "geometry", group);
            grow->setDuration(85);
            grow->setStartValue(original);
            grow->setEndValue(grown);
            grow->setEasingCurve(QEasingCurve::OutCubic);
            QPropertyAnimation *shrink = new QPropertyAnimation(targetCell, "geometry", group);
            shrink->setDuration(85);
            shrink->setStartValue(grown);
            shrink->setEndValue(original);
            shrink->setEasingCurve(QEasingCurve::InCubic);
            group->addAnimation(grow);
            group->addAnimation(shrink);
            connect(group, &QSequentialAnimationGroup::finished, this, [targetCell]() {
                targetCell->setStyleSheet(kCellStyle);
            });
            group->start(QAbstractAnimation::DeleteWhenStopped);
        }
    }

    isPlayerTurn = true;
    time = QTime(0, 2, 0);
    emit timeChanged(time.toString("mm:ss"));
    timer.start(1000);
}


void BaldaBoard::onBoardClicked(int id)
{
    if (!isStarted || !isPlayerTurn)
        return;

    QPushButton *btn =
        qobject_cast<QPushButton*>(board->button(id));

    if (!btn)
        return;

    if (!addLetter && btn->text().isEmpty()) {
        if (letter.isEmpty())
            return;

        btn->setText(letter);
        btn->setStyleSheet(kSelectedCellStyle);

        QRect original = btn->geometry();
        QRect grown = original.adjusted(-2, -2, 2, 2);
        QSequentialAnimationGroup *group = new QSequentialAnimationGroup(this);
        QPropertyAnimation *grow = new QPropertyAnimation(btn, "geometry", group);
        grow->setDuration(85);
        grow->setStartValue(original);
        grow->setEndValue(grown);
        grow->setEasingCurve(QEasingCurve::OutCubic);
        QPropertyAnimation *shrink = new QPropertyAnimation(btn, "geometry", group);
        shrink->setDuration(85);
        shrink->setStartValue(grown);
        shrink->setEndValue(original);
        shrink->setEasingCurve(QEasingCurve::InCubic);
        group->addAnimation(grow);
        group->addAnimation(shrink);
        group->start(QAbstractAnimation::DeleteWhenStopped);

        addLetter = true;
        player << btn;
        playerWord = letter;
        curPos = id;
        return;
    }

    if (btn->text().isEmpty())
        return;

    if (!player.isEmpty() && btn == player.last()) {
        btn->setStyleSheet(kCellStyle);
        player.pop_back();
        playerWord.chop(1);
        return;
    }

    if (!player.contains(btn)) {
        player << btn;
        playerWord += btn->text();
        btn->setStyleSheet(kSelectedCellStyle);
    }
}

void BaldaBoard::timeOutSlot()
{
    time = time.addSecs(-1);
    emit timeChanged(time.toString("mm:ss"));

    if (time.minute() == 0 && time.second() == 0) {
        timer.stop();
        if (isPlayerTurn)
            computerMove();
    }
}

void BaldaBoard::pause()
{
    timer.isActive() ? timer.stop() : timer.start(1000);
}
