#include "board.h"
#include <cstdint>
#include <iostream>
#include <ctype.h> 


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
    int side_index_adder = 0;
    if (half_turn % 2 == 1){
        same_pieces = black_pieces;
        other_pieces = white_pieces;
        side_index_adder = 6;
    }




    int queen_position = rays.leastSignificant(pieces[QUEEN + side_index_adder]);
    uint64_t queen_move_board = straightMoves(queen_position, all_pieces, other_pieces) | diagonalMoves(queen_position, all_pieces, other_pieces);

    moves.processMoveBoard(queen_move_board, other_pieces, queen_position, QUEEN, rays);





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


void Board::initializeFromFen(string fen){
    map<char, int> piece_map;
    piece_map['K'] = KING;
    piece_map['Q'] = QUEEN;
    piece_map['R'] = ROOK;
    piece_map['B'] = BISHOP;
    piece_map['N'] = KNIGHT;
    piece_map['P'] = PAWN;

    piece_map['k'] = KING + 6;
    piece_map['q'] = QUEEN + 6;
    piece_map['r'] = ROOK + 6;
    piece_map['b'] = BISHOP + 6;
    piece_map['n'] = KNIGHT + 6;
    piece_map['p'] = PAWN + 6;


    int index = 63;
    int i=0;
    char c = fen.at(0);
    while (c != ' '){
        if (isdigit(c)){
            index -= c - '0';
        } else if (piece_map.count(c)){
            pieces[piece_map[c]] |= 1ULL << index;
            index--;
        } 

        i++;
        c = fen.at(i);
    }
}


void Board::printBoard(){
    map<int, char> piece_map;
    piece_map[KING] = 'K';
    piece_map[QUEEN] = 'Q';
    piece_map[ROOK] = 'R';
    piece_map[BISHOP] = 'B';
    piece_map[KNIGHT] = 'N';
    piece_map[PAWN] = 'P';
    piece_map[KING + 6] = 'k';
    piece_map[QUEEN + 6] = 'q';
    piece_map[ROOK + 6] = 'r';
    piece_map[BISHOP + 6] = 'b';
    piece_map[KNIGHT + 6] = 'n';
    piece_map[PAWN + 6] = 'p';

    for (int i=63; i >= 0; i--){
        uint64_t piece_at_index = 1ULL << i;
        bool found_piece = false;
        for (int j=0; j < 12; j++){
            if (pieces[j] & piece_at_index){
                found_piece = true;
                cout << piece_map[j] << " ";
            }
        }

        if (!found_piece) {
            cout << "0 ";
        }

        if (i % 8 == 0){
            cout << "\n";
        }

        //cout << "i: " << i << "\n";
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
    //board.debug();
    board.initializeFromFen("R2r4/2B3b1/1p3k1p/7p/3P4/2N3K1/p2Q2Pp/1B6 w - - 0 1");
    board.printBoard();
    Moves board_moves = board.getMoves();
    board_moves.displayMoves();

    
}