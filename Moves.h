#include "Move.h"

class Moves{
    Move moves[150];
    int index;
    int len;


public:

    Moves(): index(0), len(0){} 
    Move getMove();
    void next();
    bool seek(int seekIndex);
    bool hasNext();
    bool isNull();
    void setMove(Move move);
    void setLast();
    int length();
    void displayMoves();        

};


