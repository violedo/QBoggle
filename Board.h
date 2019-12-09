#ifndef BOARD_H
#define BOARD_H

#include "Cube.h"

#include <QWidget>
class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = nullptr, int size = 5, const QString *cubeLetters = BIG_BOGGLE_CUBES);
    virtual ~Board();
    void shake();
    bool check(QString word);
    bool checkmouse(int i,int j);
    bool checknext(QString word,int i,int j);
    bool checknextmouse(QString word,int i,int j);
    int getsize();
    QString getcubeletter(int i,int j);
    void lock(int i,int j);
    void unlock(int i,int j);
    bool locked(int i,int j);
    void clearhighlight();
    void clearlock();
    int lasti=-1;
    int lastj=-1;
private:
    int size;
    Cube **cubes;
    QString *letters;

    inline int index(int i, int j) { return i * size + j; }

    static const QString STANDARD_CUBES[16];
    static const QString BIG_BOGGLE_CUBES[25];
/*
protected:
    virtual void mouseReleaseEvent(QMouseEvent *event);*/
};

#endif // BOARD_H
