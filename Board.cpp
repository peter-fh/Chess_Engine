#include <cstdint>
#include <iostream>
#include <ctype.h> 
#include "Board.h"
#include <fstream>


Board::Board(string fen){
    initializeFromFen(fen);
}


Moves Board::getMoves(){
    Moves moves;
    

    uint64_t white_pieces = 0;
    uint64_t black_pieces = 0; 
    for (int i=0; i < 6; i++){
        white_pieces |= pieces[i];
        black_pieces |= pieces[i+6];
    }


    uint64_t same_pieces;
    uint64_t other_pieces;
    int side_index_adder;
    int side;
    uint64_t all_pieces = white_pieces | black_pieces;

    if (half_turn % 2 == 0) {
	same_pieces = white_pieces;
	other_pieces = black_pieces;
	side_index_adder = 0;
	side = WHITE;
    }
    else {
        same_pieces = black_pieces;
        other_pieces = white_pieces;
        side_index_adder = 6;
	side = BLACK;
    }


    if (pieces[QUEEN + side_index_adder]) {
	int queen_position = bithack.leastSignificant(pieces[QUEEN + side_index_adder]);
	uint64_t queen_move_board = straightMoves(queen_position, all_pieces, other_pieces) 
				    | diagonalMoves(queen_position, all_pieces, other_pieces);
	processMoveBoard(&moves, queen_move_board, other_pieces, queen_position, QUEEN);
    }
 
    int king_position = bithack.leastSignificant(pieces[KING + side_index_adder]);
    uint64_t king_move_board = getKingMoves(king_position, same_pieces);
    processMoveBoard(&moves, king_move_board, other_pieces, king_position, KING);

    uint64_t rooks = pieces[ROOK + side_index_adder];
    processDoublePieceMoves(rooks, all_pieces, other_pieces, &moves, &Board::straightMoves, ROOK);    
   

    uint64_t bishops = pieces[BISHOP + side_index_adder]; 
    processDoublePieceMoves(bishops, all_pieces, other_pieces, &moves, &Board::diagonalMoves, BISHOP);    
 
    uint64_t knights = pieces[KNIGHT + side_index_adder]; 
    processDoublePieceMoves(knights, all_pieces, same_pieces, &moves, &Board::knightMoves, KNIGHT);    

    uint64_t pawns = pieces[PAWN + side_index_adder];
    if (side == WHITE)
	whitePawnMoves(pawns, all_pieces, other_pieces, &moves);
    else 
	blackPawnMoves(pawns, all_pieces, other_pieces, &moves);
    

    moves.seek(0);
    return moves;
}


void Board::makeMove(Move move){

     
    int side_index_adder = 0;
    if (half_turn % 2 == BLACK){
	side_index_adder = 6;
    }
    uint64_t from_board = 1ULL << move.squares[FROM];
    uint64_t to_board = 1ULL << move.squares[TO];
    pieces[move.type + side_index_adder] &= ~from_board;
    pieces[move.type + side_index_adder] |= to_board;

    if (move.take == true){
	for (int i = (6 - side_index_adder); i < 12 - side_index_adder; i++){
	    pieces[i] &= ~to_board;
	}
    }
    half_turn++;


}


void Board::unmakeMove(Move move){


    int side_index_adder = 0;
    if ((half_turn-1) % 2 == BLACK){
	side_index_adder = 6;
    }
    uint64_t from_board = 1ULL << move.squares[TO];
    uint64_t to_board = 1ULL << move.squares[FROM];
    pieces[move.type + side_index_adder] &= ~from_board;
    pieces[move.type + side_index_adder] |= to_board;

    if (move.take == true){
	pieces[6-side_index_adder + previous_taken_piece] |= to_board;
    }

    half_turn--;

}
uint64_t Board::diagonalMoves (int position, uint64_t all_pieces, uint64_t other_pieces){
    uint64_t move_board = 0ULL;

    move_board |= directionalMoves(position, NORTHEAST, all_pieces, other_pieces);
    move_board |= directionalMoves(position, SOUTHEAST, all_pieces, other_pieces);
    move_board |= directionalMoves(position, SOUTHWEST, all_pieces, other_pieces);
    move_board |= directionalMoves(position, NORTHWEST, all_pieces, other_pieces);

    return move_board;
}


int Board::turn(){
    return half_turn;
}


string Board::toString(){

    string board_string = "";
    std::map<int, char> piece_map;
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
                board_string += piece_map[j] ;
		board_string += " ";
            }
        }

        if (!found_piece) {
            board_string += "0 ";
        }

        if (i % 8 == 0){
            board_string += "\n";
        }

        //cout << "i: " << i << "\n";
    }

    return board_string;
}


string Board::bitboardToString(uint64_t board){
    string bitboard_string = "";
    for (int i=63; i >= 0; i--){
	if ((1ULL << i) & board)
	    bitboard_string += "1 ";
	else
	    bitboard_string += "0 ";

	if (i % 8 == 0)
	    bitboard_string += "\n";
    }

    return bitboard_string;
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


void Board::processMoveBoard(Moves *moves, uint64_t move_board, uint64_t other_pieces, int piece_position, int piece_type){
     
    while (move_board){
        Move move;
        int move_square = bithack.leastSignificant(move_board);
        uint64_t move_piece = 1ULL << move_square;
        move.squares[0] = piece_position;
        move.squares[1] = move_square;
        move.type = piece_type;
        move.take = move_piece & other_pieces;
        moves->setMove(move);

        move_board ^= move_piece;

   }
   
    
}


        

uint64_t Board::getKingMoves(int position, uint64_t same_pieces){
    if (position < 0)
	return 0ULL;
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

    return ~same_pieces & (king_position_mask & king_moves_mask);

    
}


uint64_t Board::knightMoves(int position, uint64_t same_pieces, uint64_t other_pieces){
    if (position < 0)
	return 0ULL;
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
    knight_move_board|= (knight & ~(
			bithack.getHorizontal(7) 
		     | bithack.getVertical(6) 
		     | bithack.getVertical(7))) << 10;
    // +17
    knight_move_board |= (knight & ~(
			 bithack.getHorizontal(7) 
		      | bithack.getHorizontal(6) 
		      | bithack.getVertical(7))) << 17;

    return knight_move_board & ~same_pieces;
}


void Board::processDoublePieceMoves(uint64_t piece, uint64_t all_pieces, uint64_t other_pieces, Moves *moves, uint64_t (Board::*pieceMoves)(int, uint64_t, uint64_t), int piece_type){
    if (!piece){
        return;
    }
    if (lonePiece(piece)){
	int piece_position = bithack.leastSignificant(piece);
	uint64_t piece_moves = (this->*pieceMoves)(piece_position, all_pieces, other_pieces);
	processMoveBoard(moves, piece_moves, other_pieces, piece_position, piece_type);
	return;
    }

    int piece1 = bithack.leastSignificant(piece); 
    int piece2 = bithack.mostSignificant(piece); 
    uint64_t piece1_move_board = (this->*pieceMoves)(piece1, all_pieces, other_pieces); 
    uint64_t piece2_move_board = (this->*pieceMoves)(piece2, all_pieces, other_pieces); 
    processMoveBoard(moves, piece1_move_board, other_pieces, piece1, piece_type);
    processMoveBoard(moves, piece2_move_board, other_pieces, piece2, piece_type);
 
 
}

const uint64_t FORWARD_PAWN_MASK = 		0x00FFFFFFFFFFFF00;
const uint64_t MOST_SIGNIFICANT_PAWN_MASK = 	0x00CCCCCCCCCCCC00;
const uint64_t LEAST_SIGNIFICANT_PAWN_MASK = 	0x0033333333333300;
const uint64_t SECOND_ROW_PAWN_MASK = 		0x000000000000FF00;
const uint64_t SEVENTH_ROW_PAWN_MASK = 		0x00FF000000000000;


void Board::whitePawnMoves(int64_t pawns, uint64_t all_pieces, uint64_t other_pieces, Moves *moves){ 
    if (!pawns)
	return;
    // mb = move board, can't be bothered to write it out
    uint64_t forward_mb = ((pawns & (FORWARD_PAWN_MASK)) << 8) & (~all_pieces); 
    uint64_t double_forward_mb = ((pawns & SECOND_ROW_PAWN_MASK) << 16) & (~all_pieces); 
    uint64_t promotion_mb = ((pawns & SEVENTH_ROW_PAWN_MASK) << 8) & (~all_pieces);

    uint64_t most_significant_take_mb = 0ULL;
    most_significant_take_mb |= (pawns & MOST_SIGNIFICANT_PAWN_MASK & ~bithack.getVertical(7)) << 9;
    most_significant_take_mb |= (pawns & MOST_SIGNIFICANT_PAWN_MASK) << 7;
    most_significant_take_mb &= other_pieces;

    uint64_t least_significant_take_mb = 0ULL;
    least_significant_take_mb |= (pawns & LEAST_SIGNIFICANT_PAWN_MASK) << 9;
    least_significant_take_mb |= (pawns & LEAST_SIGNIFICANT_PAWN_MASK & ~bithack.getVertical(0)) << 7;
    least_significant_take_mb &= other_pieces;
    
    
    while (forward_mb) {
	int pawn_position = bithack.leastSignificant(forward_mb);
	uint64_t pawn = 1ULL << pawn_position;
	int from = pawn_position - 8;
	Move pawn_move = Move(from, pawn_position, PAWN, 0);
	moves->setMove(pawn_move);
	forward_mb &= ~pawn;
    }

 
    while (double_forward_mb) {
	int pawn_position = bithack.leastSignificant(double_forward_mb);
	uint64_t pawn = 1ULL << pawn_position;
	int from = pawn_position - 16;
	Move pawn_move = Move(from, pawn_position, PAWN, 0);
	moves->setMove(pawn_move);
	double_forward_mb &= ~pawn;
    }
    
    // 7 6 5 4 3 2 1 0 
    // L R R L L R R L
    // 1 1 1 1 1 1 1 1 
    // 1 1 0 0 1 1 0 0
    
    while (most_significant_take_mb){
	int pawn_position = bithack.leastSignificant(most_significant_take_mb);
	uint64_t pawn = 1ULL << pawn_position;
	int from = -1;
	if (((pawn_position + 1) / 2) % 2 == 0)
	    from = pawn_position - 9;
	else 
	    from = pawn_position - 7;
	Move pawn_move = Move(from, pawn_position, PAWN, 1);
	moves->setMove(pawn_move);
	most_significant_take_mb &= ~pawn;
    }

    // 7 6 5 4 3 2 1 0 
    // / L L R R L L R
    // 1 1 1 1 1 1 1 1 
    // 0 0 1 1 0 0 1 1

    while (least_significant_take_mb){
	int pawn_position = bithack.leastSignificant(least_significant_take_mb);
	uint64_t pawn = 1ULL << pawn_position;
	int from = -1;
	if (((pawn_position + 1) / 2) % 2 == 0)
	    from = pawn_position - 7;
	else 
	    from = pawn_position - 9;
	Move pawn_move = Move(from, pawn_position, PAWN, 1);
	moves->setMove(pawn_move);
	least_significant_take_mb &= ~pawn;
    }
 
}


void Board::blackPawnMoves(int64_t pawns, uint64_t all_pieces, uint64_t other_pieces, Moves *moves){ 
    if (!pawns)
	return;
    // mb = move board, can't be bothered to write it out
    
    uint64_t forward_mb = ((pawns & (FORWARD_PAWN_MASK)) >> 8) & (~all_pieces); 
    uint64_t double_forward_mb = ((pawns & SECOND_ROW_PAWN_MASK) >> 16) & (~all_pieces); 
    uint64_t promotion_mb = ((pawns & SEVENTH_ROW_PAWN_MASK) >> 8) & (~all_pieces);

    // 11001100
    uint64_t most_significant_take_mb = 0ULL;
    most_significant_take_mb |= (pawns & MOST_SIGNIFICANT_PAWN_MASK & ~bithack.getVertical(7))  >> 7;
    most_significant_take_mb |= (pawns & MOST_SIGNIFICANT_PAWN_MASK) >> 9;
    most_significant_take_mb &= other_pieces;

    // 00110011
    uint64_t least_significant_take_mb = 0ULL;
    least_significant_take_mb |= (pawns & LEAST_SIGNIFICANT_PAWN_MASK) >> 7;
    least_significant_take_mb |= (pawns & LEAST_SIGNIFICANT_PAWN_MASK & ~bithack.getVertical(0)) >> 9;
    least_significant_take_mb &= other_pieces;


    while (forward_mb) {
	int pawn_position = bithack.leastSignificant(forward_mb);
	uint64_t pawn = 1ULL << pawn_position;
	int from = pawn_position + 8;
	Move pawn_move = Move(from, pawn_position, PAWN, 0);
	moves->setMove(pawn_move);
	forward_mb &= ~pawn;
    }


     while (double_forward_mb) {
	int pawn_position = bithack.leastSignificant(double_forward_mb);
	uint64_t pawn = 1ULL << pawn_position;
	int from = pawn_position + 16;
	Move pawn_move = Move(from, pawn_position, PAWN, 0);
	moves->setMove(pawn_move);
	double_forward_mb &= ~pawn;
    }

    // 7 6 5 4 3 2 1 0 
    // R L L R R L L /
    // 1 1 0 0 1 1 0 0
    // 1 1 1 1 1 1 1 1 
    // f(x):
    // 0 1 1 0 0 1 1 0

    
    while (most_significant_take_mb){
	int pawn_position = bithack.leastSignificant(most_significant_take_mb);
	uint64_t pawn = 1ULL << pawn_position;
	int from = -1;
	if (((pawn_position + 1) / 2) % 2 == 0)
	    from = pawn_position + 7;
	else 
	    from = pawn_position + 9;
	Move pawn_move = Move(from, pawn_position, PAWN, 1);
	moves->setMove(pawn_move);
	most_significant_take_mb &= ~pawn;
    }

    // 7 6 5 4 3 2 1 0 
    // / R R L L R R L
    // 0 0 1 1 0 0 1 1
    // 1 1 1 1 1 1 1 1
    // f(x):
    // 0 1 1 0 0 1 1 0

    while (least_significant_take_mb){
	int pawn_position = bithack.leastSignificant(least_significant_take_mb);
	uint64_t pawn = 1ULL << pawn_position;
	int from = -1;
	if (((pawn_position + 1) / 2) % 2 == 0)
	    from = pawn_position + 9;
	else 
	    from = pawn_position + 7;
	Move pawn_move = Move(from, pawn_position, PAWN, 1);
	moves->setMove(pawn_move);
	least_significant_take_mb &= ~pawn;
    }
 
}




void Board::initializeFromFen(string fen){
    for (int i=0; i < 12; i++){
        pieces[i] = 0ULL;
    }
    std::map<char, int> piece_map;
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
    i++;
    c = fen.at(i);

    
}


bool Board::isLegal(Move move){
    bool returnBool = true;
    std::ofstream errFile ("IllegalMoves.txt", std::ios::app);

    int from = move.squares[0];
    uint64_t from_board = 1ULL << from;
    int to = move.squares[1];
    uint64_t to_board = 1ULL << to;

    if (from < 0 || from > 63){
	errFile << "From square out of bounds: " << from << "\n";
	returnBool = false;
    }

    if (to < 0 || to > 63){
	errFile << "To square out of bounds: " << to << "\n";
	returnBool = false;
    }
    
    if (from == to){
	errFile << "From and to squares are the same: " << from << "\n";
	returnBool = false;
    }
 

    int from_x = from % 8;
    int from_y = from / 8;
    int to_x = to % 8;
    int to_y = to / 8;
    
    int dx = abs(from_x - to_x);
    int dy = abs(from_y - to_y);

    uint64_t friendly_pieces = 0;
    uint64_t opposite_pieces = 0;

    if (half_turn % 2 == WHITE){
	for (int i=0; i < 6; i++){
	    friendly_pieces |= pieces[i];
	}

	for (int i=6; i < 12; i++){
	    opposite_pieces |= pieces[i];

	}
    } else {
	for (int i=0; i < 6; i++){
	    opposite_pieces |= pieces[i];
	}

	for (int i=6; i < 12; i++){
	    friendly_pieces |= pieces[i];

	}
    }



    if (to_board & friendly_pieces){
	errFile << "To square is occupied by friendly piece: " << to << "\n";
	errFile << "To board: \n";
	errFile << bitboardToString(friendly_pieces);
	errFile << "\n";
	returnBool = false;
    }


    if (move.type == KING){
	if (dx > 1 || dy > 1){
	    errFile << "King moves too far: " << from << " to " << to << "\n";
	    returnBool = false;
	}
    }
    else if (move.type == QUEEN){
	if (!(dx == 0 || dy == 0 || abs(dx) == abs(dy))){
	    errFile << "Queen move direction is not valid: " << from << " to " << to << "\n";
	    returnBool = false;
	}

	    
    }
    else if (move.type == ROOK){
	if (dx != 0 && dy != 0){
	    errFile << "Rook move direction is not valid: " << from << " to " << to << "\n";
	    returnBool = false;
	}
    }
    else if (move.type == BISHOP){
	if (dx != dy){
	    errFile << "Bishop move direction is not valid: " << from << " to " << to << "\n";
	    returnBool = false;
	}

    }
    else if (move.type == KNIGHT){
	if (dx == 0 || dy == 0 || abs(dx) + abs(dy) != 3){
	    errFile << "Knight move is not valid: " << from << " to " << to << "\n";
	    returnBool = false;
	}

    }
    else if (move.type == PAWN){
	if (!((dx == 0 && dy == 1) || (dx == 1 && dy == 1) || (dx == 0 && dy == 2))){
	    errFile << "Pawn move is not valid: " << from << " to " << to << "\n";
	    returnBool = false;
	}
	if (abs(dx) + abs(dy) == 2 && !(to_board && opposite_pieces)){
	    errFile << "Pawn is taking when no opponent piece on square: " << from << " to " << to << "\n";
	    returnBool = false;
	}
	if ((dx == 0 && dy == 1) && to_board & (opposite_pieces & friendly_pieces)){
	    errFile << "Pawn is moving one space when blocked: " << from << " to " << to << "\n";
	    returnBool = false;

	}
	if ((dx == 0 && dy == 2) && to_board & (opposite_pieces & friendly_pieces)){
	    errFile << "Pawn is moving two spaces when blocked: " << from << " to " << to << "\n";
	    returnBool = false;
	}	

    }

    
    if (!returnBool){	
	errFile << toString();
	string move_string = move.moveCode();	
	errFile << "From: " << from << " to: " << to << "\n";
	errFile << "Move: " << move_string << "\n\n";
    }
    return returnBool;



}




void Board::debug(){
    uint64_t piece = 0x8000000000000000;

}


/* 
int main(){
    Board board;
    
    //board.debug();
    board.initializeFromFen("8/1q1q1q1q/P1P1P1P1/8/8/qqqq1q1q/1P1P1P1P/8 w - - 0 1");
    board.printBoard();
    Moves board_moves = board.getMoves();
    board_moves.displayMoves();
    
}
*/
