#include "Moves.h"
#include <cstdint>
#include <iostream>
#include <time.h>


Move* Moves::getMove(){
    return &moves[index];
}

Move* Moves::randomMove(){
    srand(time(NULL));
    Move* returnMove = new Move();
    *returnMove = moves[rand()];
    return returnMove;
}

void Moves::next(){
    index++;
}


void Moves::sort(){
    for (int i=1; i < len; i++){
        Move key = moves[i];
        int j = i-1;

        while (j >= 0 && moves[j].eval < key.eval){
            moves[j+1] = moves[j];
            j = j - 1;
        }

        moves[j+1] = key;
    }

}


bool Moves::seek(int seekIndex){
    if (seekIndex >= len){
        return false;
    }

    if (seekIndex == -1){
        seekIndex = len - 1;
        return true;
    }

    index = seekIndex;
    return true;
}

bool Moves::isNull(){
    if (moves[0].isNull())
        return true;

    return false;
}


bool Moves::hasNext(){
    if (moves[index+1].isNull())
        return false;

    return true;
}


void Moves::setMove(Move inputMove){
    moves[index] = inputMove;
    index++;
    len++;
}

void Moves::setLast(){
    len = index+1;
    index = 0;
}

int Moves::length(){
    return len;
}


std::ostream& operator<<(std::ostream& out, Moves moves){
    if (moves.isNull()){
        out << "No moves\n";
        return out;
    }

    moves.seek(0);

    out << *moves.getMove() << " ";
    Move* currentMove = moves.getMove();
    while (moves.hasNext()){
        moves.next();
        out << *moves.getMove() << " ";
    }

    out << "\n";

    moves.seek(0);

    return out;
}

