#include "board.h"
#include <cstdint>
#include <iostream>


int Board::turn(){
    return half_turn;
}


bool Board::isLegal(Move move){
    // TODO
    return true;
}


int Board::slowLeastSignificant (int num){
    for (int i=0; i < 16; i++){
        if ((1ULL << i) & num)
            return i;
    }

    return -1;
}


int Board::slowMostSignificant (int num){
    for (int i=15; i >= 0; i--){
        if ((1ULL << i) & num)
            return i;
    }

    return -1;
}


void Board::initializeBitPositions(){
    for (int i=0; i < 0xffff; i++){
        least_significant_positions[i] = slowLeastSignificant(i);
        most_significant_positions[i] = slowMostSignificant(i);
    }
}


const uint64_t LEAST_16 = 0xFFFF;
const uint64_t SECOND_LEAST_16 = 0xFFFF0000;
const uint64_t SECOND_MOST_16 = 0xFFFF00000000;
const uint64_t MOST_16 = 0xFFFF000000000000;

int Board::leastSignificant (uint64_t piece){

    if (LEAST_16 & piece){
        return least_significant_positions[piece];
    }

    if (SECOND_LEAST_16 & piece){
        return least_significant_positions[piece] + 16;
    }

    if (SECOND_MOST_16 & piece){
        return least_significant_positions[piece]+32;
    }

    if (MOST_16 & piece){
        return least_significant_positions[piece]+48;
    }

    return -1;
    
}

int Board::mostSignificant (uint64_t piece){

    if (MOST_16 & piece){
        return most_significant_positions[piece]+48;
    }

    if (SECOND_MOST_16 & piece){
        return most_significant_positions[piece]+32;
    }

    if (SECOND_LEAST_16 & piece){
        return most_significant_positions[piece] + 16;
    }

    if (LEAST_16 & piece){
        return most_significant_positions[piece];
    }

    return -1;
    
}

uint64_t Board::directionalMoves(int position, int direction, int all_pieces, int other_pieces){
    uint64_t piece = 1ULL << position;
    uint64_t move_board;
    //uint64_t directional_ray = rays.castRay(position, direction);
    uint64_t directional_ray = 1;

    int collision_position;
    if (!(directional_ray & all_pieces))
        return directional_ray ^ piece;
        
    if (direction == NORTHEAST || direction == NORTH || direction == NORTHWEST || direction == WEST)
        collision_position = leastSignificant(directional_ray & all_pieces);
    else {
        collision_position = mostSignificant(directional_ray & all_pieces);
    }
    

    
    //uint64_t collision_mask = ~rays.castRay(collision_position, direction);
    uint64_t collision_mask = 1;
    move_board = directional_ray & collision_mask;

    uint64_t significant_collision = 1ULL << collision_position;
    if (significant_collision & other_pieces){
        move_board |= significant_collision;
    }


    move_board ^= piece;

    
    return move_board;
    
}


Moves Board::determineLegalMoves(){
    Moves legal_moves;

    return legal_moves;
}

void Board::display_bitboard(uint64_t board){
    for (int i=63; i >= 0; i--){
        if ((1ULL << i) & board)
            cout << "1 ";
        else
            cout << "0";

        if (i % 8 == 0)
            cout << "\n";
    }
}


bool Board::loneBit(uint64_t piece){
    if (piece ^ (piece & -piece))
        return true;

    return false;
}


int main(){
    Board board;
    board.display_bitboard(0b1);
}