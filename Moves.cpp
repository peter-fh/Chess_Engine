#include "board.h"
#include <iostream>


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


void Moves::displayMoves(){
    if (isNull()){
        cout << "No moves\n";
        return;
    }


    while (hasNext()){
        cout << getMove().moveCode() << " ";
    }
}


void Moves::processMoveBoard(uint64_t move_board, uint64_t other_pieces, int piece_position, int piece_type, Rays rays){
    while (move_board){
        Move move;
        int move_square = rays.leastSignificant(move_board);
        move.squares[0] = piece_position;
        move.squares[1] = move_square;
        move.type = piece_type;
        move.take = (1ULL << move_square) & other_pieces;
        setMove(move);
   }
}

