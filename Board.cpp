#include "Board.h"
#include "Cube.h"
#include "iostream"
#include <QGridLayout>
#include <QTime>


const QString Board::STANDARD_CUBES[16]  = {
        "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
        "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
        "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
        "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

const QString Board::BIG_BOGGLE_CUBES[25]  = {
        "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
        "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
        "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
        "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
        "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

Board::Board(QWidget *parent, int size, const QString *cubeLetters) : QWidget(parent)
{
    this->size = size;
    this->cubes = new Cube*[size * size];
    this->letters = new QString[size * size];
    for (int i = 0; i < size * size; ++i)
        this->letters[i] = cubeLetters[i];

    QGridLayout *layout = new QGridLayout();
    layout->setMargin(20);
    layout->setSpacing(10);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            this->cubes[index(i, j)] = new Cube(this);
            layout->addWidget(this->cubes[index(i, j)], i, j, Qt::AlignmentFlag::AlignCenter);
        }
    }
    setLayout(layout);
    shake();
    // this->setStyleSheet("background-color:grey; border: 3px solid");
}

Board::~Board()
{
    if (cubes) delete[] cubes;
    if (letters) delete[] letters;
}

void Board::shake()
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    for (int i=0;i<size*size;++i)
    {
        int j=qrand()%(size*size-i)+i;
        swap(letters[i],letters[j]);
    }
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            this->cubes[index(i, j)]->setLetter(this->letters[index(i, j)].at(qrand()%6));
            if (cubes[index(i, j)]->getLetter()=="Q")
                cubes[index(i, j)]->setLetter("QU");
        }
    }
    // Shake Cubes
}

bool Board::check(QString word){
    int i=0,j=0;

    for (;i<size;++i)
        for (j=0;j<size;++j)
            if (word.startsWith(cubes[index(i,j)]->getLetter(),Qt::CaseInsensitive))
            {
                lock(i,j);
                if (checknext(word.right(word.length()-cubes[index(i,j)]->getLetter().length()),i,j))
                {
                    unlock(i,j);
                    cubes[index(i,j)]->setStyleSheet("background-color: yellow; border-radius: 15px; border: 2px solid");
                    return true;
                }
                unlock(i,j);
            };
    return false;
}

bool Board::checknext(QString word, int i, int j){
    if (word.length()==0)
        return true;
    int x[8]={-1,0,1,-1,1,-1,0,1};
    int y[8]={-1,-1,-1,0,0,1,1,1};
    for (int k=0;k<8;++k)
        if (i+x[k]>=0&&i+x[k]<size&&j+y[k]>=0&&j+y[k]<size)
            if (!locked(i+x[k],j+y[k])&&word.startsWith(cubes[index(i+x[k],j+y[k])]->getLetter(),Qt::CaseInsensitive))
            {
                lock(i+x[k],j+y[k]);
                if (checknext(word.right(word.length()-cubes[index(i+x[k],j+y[k])]->getLetter().length()),i+x[k],j+y[k]))
                {
                    unlock(i+x[k],j+y[k]);
                    cubes[index(i+x[k],j+y[k])]->setStyleSheet("background-color: yellow; border-radius: 15px; border: 2px solid");
                    return true;
                }
                unlock(i+x[k],j+y[k]);
            }
    return false;
}

int Board::getsize(){return size;}

QString Board::getcubeletter(int i,int j){
    return cubes[index(i,j)]->getLetter();
}

void Board::lock(int i,int j){
    cubes[index(i,j)]->used=true;
}

void Board::unlock(int i,int j){
    cubes[index(i,j)]->used=false;
}

bool Board::locked(int i,int j){
    return cubes[index(i,j)]->used;
}

void Board::clearhighlight(){
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            cubes[index(i,j)]->setStyleSheet("background-color: white; border-radius: 15px; border: 2px solid");
        }
    }
}
/*
void Board::mouseReleaseEvent(QMouseEvent *event){

}*//*
void Board::mousepress(Cube* addr){
    int x=(addr-cubes)/sizeof (Cube);
    newletterpressed(newletter);
    std::cout<<i<<j;
}*/

bool Board::checkmouse(int i,int j){

    if (lasti==-1||((lasti!=i||lastj!=j)&&(lasti-i)*(lasti-i)+(lastj-j)*(lastj-j)<3))
    {
        cubes[index(i,j)]->setStyleSheet("background-color: yellow; border-radius: 15px; border: 2px solid");
        lasti=i;
        lastj=j;
        return true;
    }
    return false;
}

void Board::clearlock(){
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            unlock(i,j);
}
















