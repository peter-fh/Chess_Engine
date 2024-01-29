#include "board.h"


Move Moves::getMove(){
    return moves[index];
}

void Moves::next(){
    index++;
}


bool Moves::seek(int seekIndex){
    if (seekIndex >= len){
        return false;
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
}

void Moves::setLast(){
    len = index+1;
    index = 0;
}

int Moves::length(){
    return len;
}

