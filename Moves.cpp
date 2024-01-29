#include "board.h"
#include <cstdint>
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


void Moves::displayMoves(){
    if (isNull()){
        cout << "No moves\n";
        return;
    }

    seek(0);

    cout << getMove().moveCode() << " ";
    while (hasNext()){
        next();
        cout << getMove().moveCode() << " ";
    }
}


void Moves::processMoveBoard(uint64_t move_board, uint64_t other_pieces, int piece_position, int piece_type, Rays rays){
    while (move_board){
        Move move;
        int move_square = rays.leastSignificant(move_board);
        uint64_t move_piece = 1ULL << move_square;
        move.squares[0] = piece_position;
        move.squares[1] = move_square;
        move.type = piece_type;
        move.take = move_piece & other_pieces;
        setMove(move);

        move_board ^= move_piece;
   }
}

