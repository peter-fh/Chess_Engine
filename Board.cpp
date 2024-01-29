#include "board.h"
#include <cstdint>
#include <iostream>


int Board::turn(){
    return half_turn;
}



bool Board::loneBit(uint64_t piece){
    if (piece ^ (piece & -piece))
        return true;

    return false;
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
        collision_position = rays.leastSignificant(collisions);
    else {
        collision_position = rays.mostSignificant(collisions);
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


uint64_t Board::straightMoves(int position, uint64_t all_pieces, uint64_t other_pieces){
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


Moves Board::getMoves(){
    Moves moves;

    uint64_t white_pieces = 0;
    uint64_t black_pieces = 0; 
    for (int i=0; i < 6; i++){
        white_pieces |= pieces[i];
        black_pieces |= pieces[i+6];
    }

    uint64_t all_pieces = white_pieces | black_pieces;
    uint64_t same_pieces = white_pieces;
    uint64_t other_pieces = black_pieces;
    if (half_turn % 2 == 1){
        same_pieces = black_pieces;
        other_pieces = white_pieces;
    }


    return moves;
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


void Board::debug(){
    uint64_t piece = 0x0000000200000000;
    uint64_t other_pieces = 0xFF818181818181FF;
    uint64_t all_pieces =  other_pieces | piece;
    
    int piece_position = rays.leastSignificant(piece);

    
    cout << "All pieces:\n";
    display_bitboard(all_pieces);

    cout << "Other pieces:\n";
    display_bitboard(other_pieces);
    
    cout << "Straight:\n";
    uint64_t straight_move_board = straightMoves(piece_position, all_pieces, other_pieces);
    display_bitboard(straight_move_board);

    cout << "Diagonal:\n";
    uint64_t diagonal_move_board = diagonalMoves(piece_position, all_pieces, other_pieces);
    display_bitboard(diagonal_move_board);

    Moves moves;
    moves.processMoveBoard(straight_move_board, other_pieces, piece_position, 1, rays);
    moves.processMoveBoard(diagonal_move_board, other_pieces, piece_position, 1, rays);
    
    moves.displayMoves();


}

int main(){
    Board board;
    board.debug();

    
}