#include "BoggleWindow.h"
#include "lexicon.h"

#include <QFile>
#include <QHBoxLayout>
#include <QTextEdit>
#include <iostream>

BoggleWindow::BoggleWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("QBoggle!");
    this->setFixedSize(BOGGLE_WINDOW_WIDTH, BOGGLE_WINDOW_HEIGHT);

    me = new WordListWidget(this, "Me");
    computer = new WordListWidget(this, "Computer");
    board = new Board(this);
    console = new Console(this);

    me->setGeometry(20, 20, 230, 300);
    board->setGeometry(230, 0, 300, 300);
    computer->setGeometry(800 - 50 - 200, 20, 230, 300);
    console->setGeometry(30, 320, 740, 260);

    QFile qFile(":/res/EnglishWords.txt");
    if (!qFile.open(QIODevice::ReadOnly)) {
        throw new std::runtime_error("Resource file not found!");
    }
    lex=new Lexicon(qFile);
/*

    for (std::string s: lex) {
        std::cout << s << std::endl;
    }
*/
    console->write("Welcome to the game Boggle!\n");
    connect(console,SIGNAL(newLineWritten(QString)),this, SLOT(check(QString)));
}

void BoggleWindow::restart(){
    me->stupid();
    computer->stupid();
    board->shake();
    console->clear();
    console->write("Welcome to the game Boggle!\n");
}

BoggleWindow::~BoggleWindow()
{
    delete me;
    delete computer;
    delete board;
    delete console;
    delete lex;
}


void BoggleWindow::check(QString word){
    board->clearhighlight();
    if (word.length()==0)
    {
        if (!computered)
        {
            computerturn();
            computered=true;
        }
        else {
            restart();
            computered=false;
        }
        return;
    }
    if (word.length()<4||checkused(word)||!checklex(word)||!checkmap(word))
    {
        console->write("Illegal word.");
        return;
    }
    me->addWord(word);
    me->addScore(word.length()-3);
}

bool BoggleWindow::checkused(QString word){
    return me->checkused(word);
}

bool BoggleWindow::checkmap(QString word){
    return board->check(word);
}

bool BoggleWindow::checklex(QString word){
    return lex->contains(word.toStdString());
}

void BoggleWindow::computerturn(){
    for (int i=0;i<board->getsize();++i)
        for (int j=0;j<board->getsize();++j)
        {

            board->lock(i,j);
            findword(board->getcubeletter(i,j),i,j);
            board->unlock(i,j);
        }
    console->write("Game over!Press Enter to restart.\n");
}

void BoggleWindow::findword(QString prefix, int i, int j){
    if (!lex->containsPrefix(prefix.toStdString()))
        return;
    if (prefix.length()>3&&!me->checkused(prefix)&&!computer->checkused(prefix)&&lex->contains(prefix.toStdString()))
    {
        computer->addWord(prefix);
        computer->addScore(prefix.length()-3);
    }
    int x[8]={-1,0,1,-1,1,-1,0,1};
    int y[8]={-1,-1,-1,0,0,1,1,1};
    for (int k=0;k<8;++k)
        if (i+x[k]>=0&&i+x[k]<board->getsize()&&j+y[k]>=0&&j+y[k]<board->getsize())
            if (!board->locked(i+x[k],j+y[k]))
            {
                board->lock(i+x[k],j+y[k]);
                findword(prefix+board->getcubeletter(i+x[k],j+y[k]),i+x[k],j+y[k]);
                board->unlock(i+x[k],j+y[k]);
            }
}

void BoggleWindow::mouseReleaseEvent(QMouseEvent *event){
    if (event->x()>=260&&event->x()<=530&&event->y()>=30&&event->y()<=300)
    {
        int i=(event->y()-30)/54,j=(event->x()-260)/54;
        if (board->lasti==-1)
            board->clearhighlight();
        mouseword+=board->getcubeletter(i,j);


        if (board->locked(i,j)||!board->checkmouse(i,j)||checkused(mouseword)||!lex->containsPrefix(mouseword.toStdString()))
        {
            console->write("Illegal word.");
            board->clearhighlight();
            board->clearlock();
            mouseword.clear();
            board->lasti=-1;
            board->lastj=-1;
            return;
        }
         board->lock(i,j);
        if (mouseword.length()>3&&checklex(mouseword))
        {
            me->addWord(mouseword);
            me->addScore(mouseword.length()-3);
            board->clearlock();
            mouseword.clear();
            board->lasti=-1;
            board->lastj=-1;
        }
    }
}


























