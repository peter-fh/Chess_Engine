#include "board.h"
#include <cstdint>
#include <iostream>


int Board::turn(){
    return half_turn;
}


bool Board::isLegal(Move move){
    // TODO
    if (move.equals(move))
        return true;
    return false;
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


const uint64_t LEAST_16 =           0x000000000000FFFF;
const uint64_t SECOND_LEAST_16 =    0x00000000FFFF0000;
const uint64_t SECOND_MOST_16 =     0x0000FFFF00000000;
const uint64_t MOST_16 =            0xFFFF000000000000;

int Board::leastSignificant(uint64_t piece){

    if (LEAST_16 & piece){
        piece &= LEAST_16;
        return least_significant_positions[piece];
    }

    if (SECOND_LEAST_16 & piece){
        piece &= SECOND_LEAST_16;
        return least_significant_positions[piece >> 16] + 16;
    }

    if (SECOND_MOST_16 & piece){
        piece &= SECOND_MOST_16;
        return least_significant_positions[piece >> 32]+32;
    }

    if (MOST_16 & piece){
        piece &= MOST_16;
        return least_significant_positions[piece >> 48]+48;
    }

    return -1;
    
}


int Board::mostSignificant (uint64_t piece){

    if (MOST_16 & piece){
        return most_significant_positions[piece >> 48]+48;
    }

    if (SECOND_MOST_16 & piece){
        return most_significant_positions[piece >> 32]+32;
    }

    if (SECOND_LEAST_16 & piece){
        return most_significant_positions[piece >> 16] + 16;
    }

    if (LEAST_16 & piece){
        return most_significant_positions[piece];
    }

    return -1;
    
}


uint64_t Board::directionalMoves(int position, int direction, uint64_t all_pieces, uint64_t other_pieces){
    uint64_t piece = 1ULL << position;
    uint64_t move_board;
    uint64_t directional_ray = rays.castRay(position, direction);

    int collision_position;
    uint64_t collisions = directional_ray & (all_pieces ^ piece);


    if (!collisions){  
        return directional_ray ^ piece;
    }
        
    if (direction > 0)
        collision_position = leastSignificant(collisions);
    else {
        collision_position = mostSignificant(collisions);
    }
    
    uint64_t collision_mask = ~rays.castRay(collision_position, direction);
    move_board = directional_ray & collision_mask;

    uint64_t significant_collision = 1ULL << collision_position;
    if (significant_collision & other_pieces){
        move_board |= significant_collision;
    }


    move_board ^= piece;

    
    return move_board;
    
}


uint64_t Board::horizontalMoves (int position, uint64_t all_pieces, uint64_t other_pieces){
    uint64_t move_board = 0ULL;

    move_board |= directionalMoves(position, NORTH, all_pieces, other_pieces);
    move_board |= directionalMoves(position, EAST, all_pieces, other_pieces);
    move_board |= directionalMoves(position, SOUTH, all_pieces, other_pieces);
    move_board |= directionalMoves(position, WEST, all_pieces, other_pieces);

    return move_board;
}


uint64_t Board::diagonalMoves (int position, uint64_t all_pieces, uint64_t other_pieces){
    uint64_t move_board = 0ULL;

    move_board |= directionalMoves(position, NORTHEAST, all_pieces, other_pieces);
    move_board |= directionalMoves(position, SOUTHEAST, all_pieces, other_pieces);
    move_board |= directionalMoves(position, SOUTHWEST, all_pieces, other_pieces);
    move_board |= directionalMoves(position, NORTHWEST, all_pieces, other_pieces);

    return move_board;
}


void Board::display_bitboard(uint64_t board){
    for (int i=63; i >= 0; i--){
        if ((1ULL << i) & board)
            cout << "1 ";
        else
            cout << "0 ";

        if (i % 8 == 0)
            cout << "\n";
    }
}


bool Board::loneBit(uint64_t piece){
    if (piece ^ (piece & -piece))
        return true;

    return false;
}


void Board::debug(){
    uint64_t piece = 0x0000000200000000;
    uint64_t all_pieces = 0xFF818181818181FF | piece;
    uint64_t other_pieces = 0;
    int piece_position = leastSignificant(piece);


    
    cout << "All pieces:\n";
    display_bitboard(all_pieces);

    cout << "Other pieces:\n";
    display_bitboard(other_pieces);
    
    cout << "Horizontal:\n";
    display_bitboard(horizontalMoves(piece_position, all_pieces, other_pieces));
    


    cout << "Diagonal:\n";
    display_bitboard(diagonalMoves(piece_position, all_pieces, other_pieces));

    
}

int main(){
    Board board;
    board.debug();

    
}