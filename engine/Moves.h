#include "Move.h"


const int MOVES_LENGTH = 100;


class Moves{
    
public:
    Moves(): index(0), len(0){} 
    Move* getMove();
    Move* randomMove();
    void next();
    bool seek(int seekIndex);
    bool hasNext();
    bool isNull();
    void setMove(Move move);
    void setLast();
    int length();
    friend std::ostream& operator<<(std::ostream& out, Moves moves);
    void sort();
    void displayMoves();        

private:
    Move moves[MOVES_LENGTH];
    int index;
    int len;
};


class iterator{
    int index;
    iterator& operator++();
    iterator operator*();
    bool operator!=(const iterator& other);
};


