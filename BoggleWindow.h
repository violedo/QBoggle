#ifndef BOGGLEWINDOW_H
#define BOGGLEWINDOW_H

#include "Board.h"
#include "Console.h"
#include "WordListWidget.h"
#include "lexicon.h"
#include <QMouseEvent>
#include <QMainWindow>

class BoggleWindow : public QMainWindow
{
    Q_OBJECT

public:
    BoggleWindow(QWidget *parent = 0);
    ~BoggleWindow();

public slots:
    void check(QString);
private:
    bool checkused(QString);
    bool checkmap(QString);
    bool checklex(QString);
    void computerturn();
    void findword(QString prefix,int i,int j);
    void restart();

    QString mouseword;
    WordListWidget *me;
    WordListWidget *computer;
    bool computered=false;
    Board *board;
    Console *console;
    Lexicon *lex;
    static const int BOGGLE_WINDOW_WIDTH = 800;
    static const int BOGGLE_WINDOW_HEIGHT = 600;

protected:
    virtual void mouseReleaseEvent(QMouseEvent *event);
};

#endif // BOGGLEWINDOW_H
