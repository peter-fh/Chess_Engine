#include "Moves.h"
#include <cstdint>
#include <iostream>
#include <time.h>


Move Moves::getMove(){
    return moves[index];
}

Move Moves::randomMove(){
    srand(time(NULL));
    return moves[rand() % len];
}

void Moves::next(){
    index++;
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

    out << moves.getMove().moveCode() << " ";
    while (moves.hasNext()){
        moves.next();
        out << moves.getMove().moveCode() << " ";
    }

    out << "\n";

    moves.seek(0);

    return out;
}

