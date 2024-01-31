#include "board.h"
#include <cstdint>
#include <iostream>
#include <ctype.h> 

Bithack bithack;

int Board::turn(){
    return half_turn;
}



bool Board::lonePiece(uint64_t piece){
    if (piece ^ (piece & -piece))
        return false;

    return true;
}


uint64_t Board::directionalMoves(int position, int direction, uint64_t all_pieces, uint64_t other_pieces){
    if (position < 0)
        return 0ULL;
    uint64_t piece = 1ULL << position;
    uint64_t move_board;
    uint64_t directional_ray = bithack.castRay(position, direction);

    int collision_position;
    uint64_t collisions = directional_ray & (all_pieces ^ piece);


    if (!collisions){  
        return directional_ray ^ piece;
    }
        
    if (direction > 0)
        collision_position = bithack.leastSignificant(collisions);
    else {
        collision_position = bithack.mostSignificant(collisions);
    }
    
    uint64_t collision_mask = ~bithack.castRay(collision_position, direction);
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


uint64_t Board::getKingMoves(int position, uint64_t all_pieces, uint64_t other_pieces){
    int x = position % 8;
    int y = position / 8;
    uint64_t king_moves_mask = 0;
    if (x != 0)
        king_moves_mask |= bithack.getVertical(position-1);
    if (x != 7)
        king_moves_mask |= bithack.getVertical(position+1);
    if (y != 0)
        king_moves_mask |= bithack.getHorizontal(position-8);
    if (y != 7)
        king_moves_mask |= bithack.getHorizontal(position+8);


    uint64_t king_position_mask = 0;
    king_position_mask |= bithack.getCounterDiagonal(position)
                        | bithack.getDiagonal(position) 
                        | bithack.getHorizontal(position)
                        | bithack.getVertical(position);


    return king_position_mask & king_moves_mask;

    
}


uint64_t Board::getKnightMoves(int position, uint64_t same_pieces){
    uint64_t knight = 1ULL << position;
    uint64_t knight_move_board = 0ULL;
    //+15 (up right)
    knight_move_board |= (knight & ~(
                        bithack.getHorizontal(7) 
                      | bithack.getHorizontal(6) 
                      | bithack.getVertical(0)))  << 15;
    // +6 (right up)
    knight_move_board|= (knight & ~(
                       bithack.getHorizontal(7) 
                     | bithack.getVertical(1) 
                     | bithack.getVertical(0))) << 6;
    // -17
    knight_move_board |= (knight & ~(
                        bithack.getHorizontal(0)
                      | bithack.getHorizontal(1) 
                      | bithack.getVertical(0))) >> 17;
    // -10 
    knight_move_board|= (knight & ~(
                       bithack.getHorizontal(0) 
                     | bithack.getVertical(0) 
                     | bithack.getVertical(1))) >> 10;
    // -15
    knight_move_board|= (knight & ~(
                        bithack.getHorizontal(0) 
                     | bithack.getHorizontal(1) 
                     | bithack.getVertical(7))) >> 15;
    // -6
    knight_move_board|= (knight & ~(
                       bithack.getHorizontal(0) 
                     | bithack.getVertical(6) 
                     | bithack.getVertical(7))) >> 6;
    // +10
    knight_move_board|= (knight & ~(bithack.getHorizontal(7) | bithack.getVertical(6) | bithack.getVertical(7))) << 10;
    // +17
    knight_move_board |= (knight & ~(bithack.getHorizontal(7) | bithack.getHorizontal(6) | bithack.getVertical(7))) << 17;

    return knight_move_board & ~same_pieces;
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


    int queen_position = bithack.leastSignificant(pieces[QUEEN + side_index_adder]);
    uint64_t queen_move_board = straightMoves(queen_position, all_pieces, other_pieces) 
                                | diagonalMoves(queen_position, all_pieces, other_pieces);
    moves.processMoveBoard(queen_move_board, other_pieces, queen_position, QUEEN);

    int king_position = bithack.leastSignificant(pieces[KING + side_index_adder]);
    uint64_t king_move_board = getKingMoves(king_position, all_pieces, other_pieces);
    moves.processMoveBoard(king_move_board, other_pieces, king_position, KING);


    uint64_t rooks = pieces[ROOK + side_index_adder];
    if (lonePiece(rooks)){
        int rook_position = bithack.leastSignificant(rooks);
        uint64_t rook_move_board = straightMoves(rook_position, all_pieces, other_pieces);
        moves.processMoveBoard(rook_move_board, other_pieces, rook_position, ROOK);
    } else if (rooks){
        int rook1 = bithack.leastSignificant(rooks);
        int rook2 = bithack.mostSignificant(rooks);
        uint64_t rook1_move_board = straightMoves(rook1, all_pieces, other_pieces);
        uint64_t rook2_move_board = straightMoves(rook2, all_pieces, other_pieces);
        moves.processMoveBoard(rook1_move_board, other_pieces, rook1, ROOK);
        moves.processMoveBoard(rook2_move_board, other_pieces, rook2, ROOK);
    }
    
    
    uint64_t bishops = pieces[BISHOP + side_index_adder];
    if (lonePiece(bishops)){
        int bishop_position = bithack.leastSignificant(bishops);
        uint64_t bishop_move_board = diagonalMoves(bishop_position, all_pieces, other_pieces);
        moves.processMoveBoard(bishop_move_board, other_pieces, bishop_position, BISHOP);
    } else if (bishops){
        int bishop1 = bithack.leastSignificant(bishops);
        int bishop2 = bithack.mostSignificant(bishops);
        uint64_t bishop1_move_board = diagonalMoves(bishop1, all_pieces, other_pieces);
        uint64_t bishop2_move_board = diagonalMoves(bishop2, all_pieces, other_pieces);
        moves.processMoveBoard(bishop1_move_board, other_pieces, bishop1, BISHOP);
        moves.processMoveBoard(bishop2_move_board, other_pieces, bishop2, BISHOP);
    }
    
 
    uint64_t knights = pieces[KNIGHT + side_index_adder];
    if (lonePiece(knights)){
        int knight_position = bithack.leastSignificant(knights);
        uint64_t knight_move_board = getKnightMoves(knight_position, same_pieces);
        moves.processMoveBoard(knight_move_board, other_pieces, knight_position, KNIGHT);
    } else if (knights){
        int knight1= bithack.leastSignificant(knights);
        int knight2 = bithack.mostSignificant(knights);
        uint64_t knight1_move_board = getKnightMoves(knight1, same_pieces);
        uint64_t knight2_move_board = getKnightMoves(knight2, same_pieces);
        moves.processMoveBoard(knight1_move_board, other_pieces, knight1, KNIGHT);
        moves.processMoveBoard(knight2_move_board, other_pieces, knight2, KNIGHT);
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
    uint64_t piece = 0x8000000000000000;
    


}


int main(){
    Board board;
    
    //board.debug();
    board.initializeFromFen("R2r4/2B3b1/1p3k1p/7p/3P4/2N3K1/p2Q2Pp/1B6 w - - 0 1");
    board.printBoard();
    //cout << "???\n";
    Moves board_moves = board.getMoves();
    board_moves.displayMoves();

    
}