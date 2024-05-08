#include <algorithm>
#include <stdlib.h>
#include <cstdint>
#include <iostream>
#include <ctype.h> 
#include "Board.h"
#include <fstream>
#include <sstream>
#include <memory>


Board::Board(Fen fen){
    initializeFromFen(fen);
}


string Board::fen(){
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
    string return_fen = "";
    int empty_squares = 0;
    for (int i=63; i >= 0; i--){
	uint64_t piece = 1ULL << i;
	bool found_piece = false;
	for (int i=0; i < 12; i++){
	    if (piece & pieces[i]){
		found_piece = true;
		if (empty_squares != 0){
		    return_fen += std::to_string(empty_squares);
		}
		return_fen += piece_map[i];
		empty_squares = 0;
	    } 
	}
	if (!found_piece){
	    empty_squares++;
	} else {
	    empty_squares = 0;
	}
	if ((i) % 8 == 0 && i != 63 && i != 0){
	    if (empty_squares != 0){
		return_fen += std::to_string(empty_squares);
		empty_squares = 0;
	    }
	    return_fen += "/";
	}
    }
    if (empty_squares != 0){
	return_fen += std::to_string(empty_squares);
    }
    return_fen += " ";
    int current_turn = half_turn;
    if (current_turn % 2 == WHITE){
	return_fen += "w";
    } else {
	return_fen += "b";
    }

    return return_fen;
}


void Board::initializeSidedBoard(){
    
    uint64_t white_pieces = 0ULL;
    uint64_t black_pieces = 0ULL; 
    for (int i=0; i < 6; i++){
        white_pieces |= pieces[i];
        black_pieces |= pieces[i+6];
    }


    sided_board.all_pieces = white_pieces | black_pieces;

    if (half_turn % 2 == WHITE) {
	sided_board.same_pieces = white_pieces;
	sided_board.other_pieces = black_pieces;
	
    }
    else {
	sided_board.same_pieces = black_pieces;
	sided_board.other_pieces = white_pieces;
	
    }

}


#define KING_INDEX half_turn % 2 == 0 ? KING : KING + 6
#define QUEEN_INDEX half_turn % 2 == 0 ? QUEEN : QUEEN + 6
#define ROOK_INDEX half_turn % 2 == 0 ? ROOK : ROOK + 6
#define BISHOP_INDEX half_turn % 2 == 0 ? BISHOP : BISHOP + 6
#define KNIGHT_INDEX half_turn % 2 == 0 ? KNIGHT : KNIGHT + 6
#define PAWN_INDEX half_turn % 2 == 0 ? PAWN : PAWN + 6
// TODO: organize based on potential
Moves* Board::getMoves(){
    next_moves = new Moves();

    SidedBoard* sided_board = new SidedBoard();
    initializeSidedBoard();


    if (pieces[QUEEN_INDEX]) {
	int queen_position = 
	    bithack.leastSignificant(pieces[QUEEN_INDEX]);

	uint64_t queen_move_board = 
	    straightMoves(queen_position) | diagonalMoves(queen_position);

	processMoveBoard(queen_move_board, queen_position, QUEEN);
    }
 
    int king_position = bithack.leastSignificant(pieces[KING_INDEX]);

    uint64_t king_move_board = getKingMoves(king_position);
    processMoveBoard(king_move_board, king_position, KING);


    uint64_t rooks = pieces[ROOK_INDEX];
    processDoublePieceMoves(rooks, ROOK, &Board::straightMoves);    
   

    uint64_t bishops = pieces[BISHOP_INDEX]; 
    processDoublePieceMoves(bishops, BISHOP, &Board::diagonalMoves);    
 
    uint64_t knights = pieces[KNIGHT_INDEX]; 
    processDoublePieceMoves(knights, KNIGHT, &Board::knightMoves);

    uint64_t pawns = pieces[PAWN_INDEX];
    if (half_turn % 2 == 0)
	whitePawnMoves(pawns);
    else 
	blackPawnMoves(pawns);
    

    next_moves->seek(0);
    return next_moves;
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

    if (move.take >= 0){
	pieces[move.take + 6 - side_index_adder] &= ~to_board;
    }
    half_turn++;


}


void Board::unmakeMove(Move move){


    int side_index_adder = 0;
    if ((half_turn-1) % 2 == BLACK){
	side_index_adder = 6;
    }
    uint64_t from_board = 1ULL << move.squares[FROM];
    uint64_t to_board = 1ULL << move.squares[TO];
    pieces[move.type + side_index_adder] &= ~to_board;
    pieces[move.type + side_index_adder] |= from_board;

    if (move.take >= 0){
	pieces[6-side_index_adder + move.take] |= to_board;
    }

    half_turn--;

}
uint64_t Board::diagonalMoves (int position){
    uint64_t move_board = 0ULL;

    move_board |= 
	directionalMoves(position, NORTHEAST);
    move_board |= 
	directionalMoves(position, SOUTHEAST);
    move_board |= 
	directionalMoves(position, SOUTHWEST);
    move_board |= 
	directionalMoves(position, NORTHWEST);

    return move_board;
}


int Board::turn(){
    return half_turn;
}


ostream& operator<<(ostream &out, const Board &board){

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
            if (board.pieces[j] & piece_at_index){
                found_piece = true;
		out << piece_map[j];
		out << " ";
            }
        }

        if (!found_piece) {
	    out << "0 ";
        }

        if (i % 8 == 0){
	    out << "\n";
        }

        //cout << "i: " << i << "\n";
    }

    out << "\nHalf turn: " << std::to_string(board.half_turn) << "\n"; 

    return out;
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

bool Board::validBoardState(){
    for (int i=0; i < 12; i++){
	for (int j=0; j < 12; j++){
	    if (j != i && pieces[i] & pieces[j]){
		std::cerr << "i: " << i << " j: " << j << "\n";
		std::cerr << bitboardToString(pieces[i]) << "\n" ;
		std::cerr << bitboardToString(pieces[j]) << "\n";
		return false;
	    }
	}
    }

    /* if (!pieces[0] || !pieces[6])
	return false; */
    return true;
}


int Board::evaluatePiece(uint64_t board, int* table, int evaluation){
    int eval = 0;
    int side_multiplier = 1;
    if (abs(evaluation) != evaluation){
	side_multiplier = -1;
    }
    while (board){
	int pos = bithack.leastSignificant(board);
	eval += table[63-pos] * side_multiplier + evaluation;
	board &= ~(1ULL << pos);
    }

    return eval;
}


int Board::evaluate(){
    int evaluation = 0;
    evaluation += evaluatePiece(pieces[KING], pst.white_king, 20000);
    evaluation += evaluatePiece(pieces[QUEEN], pst.white_queen, 800);
    evaluation += evaluatePiece(pieces[ROOK], pst.white_rook, 500);
    evaluation += evaluatePiece(pieces[BISHOP], pst.white_bishop, 315);
    evaluation += evaluatePiece(pieces[KNIGHT], pst.white_knight, 300);
    evaluation += evaluatePiece(pieces[PAWN], pst.white_pawn, 100);
    evaluation += evaluatePiece(pieces[KING + 6], pst.black_king, -20000);
    evaluation += evaluatePiece(pieces[QUEEN + 6], pst.black_queen, -800);
    evaluation += evaluatePiece(pieces[ROOK + 6], pst.black_rook, -500);
    evaluation += evaluatePiece(pieces[BISHOP + 6], pst.black_bishop, -315);
    evaluation += evaluatePiece(pieces[KNIGHT + 6], pst.black_knight, -300);
    evaluation += evaluatePiece(pieces[PAWN + 6], pst.black_pawn, -100);
    return evaluation;
}





bool Board::lonePiece(uint64_t piece){
    if (piece ^ (piece & -piece))
        return false;
        
    return true;
}


uint64_t Board::directionalMoves(int position, int direction){

    if (position < 0)
        return 0ULL;
    uint64_t piece = 1ULL << position;
    uint64_t move_board;
    uint64_t directional_ray = bithack.castRay(position, direction);

    int collision_position;
    uint64_t collisions = directional_ray & (sided_board.all_pieces ^ piece);



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
    if (significant_collision & sided_board.other_pieces){
        move_board |= significant_collision;
    }


    move_board ^= piece;

    
    return move_board;
    
}


uint64_t Board::straightMoves(int position){

    uint64_t move_board = 0ULL;

    move_board |= directionalMoves(position, NORTH);
    move_board |= directionalMoves(position, EAST);
    move_board |= directionalMoves(position, SOUTH);
    move_board |= directionalMoves(position, WEST);

    return move_board;
}


void Board::processMoveBoard(uint64_t move_board, int piece_position, int piece_type){

    //std::cout << bitboardToString(move_board) << "\n";
    while (move_board){
        Move move;
        int move_square = bithack.leastSignificant(move_board);
        uint64_t move_piece = 1ULL << move_square;
        move.squares[0] = piece_position;
        move.squares[1] = move_square;
        move.type = piece_type;
	if ((move_piece & sided_board.other_pieces) != 0){
	    move.take = detectTakenPiece(move_piece);
	    if (move.take == -1){
		std::cout << "Exited from detectTakenPiece\n";
		std::cout << bitboardToString(sided_board.other_pieces);
		std::cout << "Half turn: " << half_turn << "\n";
		std::cout << "Move: " << move.moveCode() << "\n";
		exit(0);
	    }
	}
	else {
	    move.take = -1;
	}
        next_moves->setMove(move);

        move_board ^= move_piece;

   }
   
    
}


        

uint64_t Board::getKingMoves(int position){
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

    return ~sided_board.same_pieces & (king_position_mask & king_moves_mask);

    
}


uint64_t Board::knightMoves(int position){

    uint64_t same_pieces = sided_board.all_pieces & ~sided_board.other_pieces;
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


int Board::detectTakenPiece(uint64_t move_board){
    int other_side_index_adder = 6;
    if (half_turn % 2 == 1){
	other_side_index_adder = 0;
    }
    for (int i=other_side_index_adder ; i < 6+other_side_index_adder ; i++){
	if ((pieces[i] & move_board) != 0){
	    return i % 6;
	}
    } 


    return -1;

    
}


void Board::processDoublePieceMoves(
    uint64_t piece,  int piece_type,
    uint64_t (Board::*pieceMoves)(int)){

    if (!piece){
        return;
    }
    if (lonePiece(piece)){
	int piece_position = bithack.leastSignificant(piece);
	uint64_t piece_moves = 
	    (this->*pieceMoves)(piece_position);
	processMoveBoard(piece_moves, piece_position, piece_type);
	return;
    }

    int piece1 = bithack.leastSignificant(piece); 
    int piece2 = bithack.mostSignificant(piece); 
    uint64_t piece1_move_board = 
	(this->*pieceMoves)(piece1); 
    uint64_t piece2_move_board = 
	(this->*pieceMoves)(piece2); 
    processMoveBoard(piece1_move_board, piece1, piece_type);
    processMoveBoard(piece2_move_board, piece2, piece_type);
 
 
}

const uint64_t FORWARD_PAWN_MASK = 		0x00FFFFFFFFFFFF00;
const uint64_t MOST_SIGNIFICANT_PAWN_MASK = 	0x00CCCCCCCCCCCC00;
const uint64_t LEAST_SIGNIFICANT_PAWN_MASK = 	0x0033333333333300;
const uint64_t SECOND_ROW_PAWN_MASK = 		0x000000000000FF00;
const uint64_t SEVENTH_ROW_PAWN_MASK = 		0x00FF000000000000;


void Board::whitePawnMoves(uint64_t pawns){ 
    if (!pawns)
	return;
    // mb = move board, can't be bothered to write it out
    uint64_t forward_mb = 
	((pawns & (FORWARD_PAWN_MASK)) << 8) & (~sided_board.all_pieces); 
    uint64_t double_forward_mb = 
	((pawns & SECOND_ROW_PAWN_MASK) << 16) & (~sided_board.all_pieces); 
    uint64_t promotion_mb = 
	((pawns & SEVENTH_ROW_PAWN_MASK) << 8) & (~sided_board.all_pieces);

    uint64_t most_significant_take_mb = 0ULL;
    most_significant_take_mb |= 
	(pawns & MOST_SIGNIFICANT_PAWN_MASK & ~bithack.getVertical(7)) << 9;
    most_significant_take_mb |= (pawns & MOST_SIGNIFICANT_PAWN_MASK) << 7;
    most_significant_take_mb &= sided_board.other_pieces;

    uint64_t least_significant_take_mb = 0ULL;
    least_significant_take_mb |= (pawns & LEAST_SIGNIFICANT_PAWN_MASK) << 9;
    least_significant_take_mb |= 
	(pawns & LEAST_SIGNIFICANT_PAWN_MASK & ~bithack.getVertical(0)) << 7;
    least_significant_take_mb &= sided_board.other_pieces;
    
    
    while (forward_mb) {
	int pawn_position = bithack.leastSignificant(forward_mb);
	uint64_t pawn = 1ULL << pawn_position;
	int from = pawn_position - 8;
	Move pawn_move = Move(from, pawn_position, PAWN, -1);
	next_moves->setMove(pawn_move);
	forward_mb &= ~pawn;
    }

 
    while (double_forward_mb) {
	int pawn_position = bithack.leastSignificant(double_forward_mb);
	uint64_t pawn = 1ULL << pawn_position;
	int from = pawn_position - 16;
	Move pawn_move = Move(from, pawn_position, PAWN, -1);
	next_moves->setMove(pawn_move);
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

	int taken_position = detectTakenPiece(pawn);
	Move pawn_move = Move(from, pawn_position, PAWN, taken_position);
	next_moves->setMove(pawn_move);
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
	int taken_position = detectTakenPiece(pawn);
	Move pawn_move = Move(from, pawn_position, PAWN, taken_position);
	next_moves->setMove(pawn_move);
	least_significant_take_mb &= ~pawn;
    }
 
}


void Board::blackPawnMoves(uint64_t pawns){
    if (!pawns)
	return;
    // mb = move board, can't be bothered to write it out
    
    uint64_t forward_mb = ((pawns & (FORWARD_PAWN_MASK)) >> 8) & (~sided_board.all_pieces); 
    uint64_t double_forward_mb = 
	((pawns & SECOND_ROW_PAWN_MASK) >> 16) & (~sided_board.all_pieces); 
    uint64_t promotion_mb = 
	((pawns & SEVENTH_ROW_PAWN_MASK) >> 8) & (~sided_board.all_pieces);

    // 11001100
    uint64_t most_significant_take_mb = 0ULL;
    most_significant_take_mb |= 
	(pawns & MOST_SIGNIFICANT_PAWN_MASK & ~bithack.getVertical(7))  >> 7;
    most_significant_take_mb |= (pawns & MOST_SIGNIFICANT_PAWN_MASK) >> 9;
    most_significant_take_mb &= sided_board.other_pieces;

    // 00110011
    uint64_t least_significant_take_mb = 0ULL;
    least_significant_take_mb |= (pawns & LEAST_SIGNIFICANT_PAWN_MASK) >> 7;
    least_significant_take_mb |= 
	(pawns & LEAST_SIGNIFICANT_PAWN_MASK & ~bithack.getVertical(0)) >> 9;
    least_significant_take_mb &= sided_board.other_pieces;


    while (forward_mb) {
	int pawn_position = bithack.leastSignificant(forward_mb);
	uint64_t pawn = 1ULL << pawn_position;
	int from = pawn_position + 8;
	Move pawn_move = Move(from, pawn_position, PAWN, -1);
	next_moves->setMove(pawn_move);
	forward_mb &= ~pawn;
    }


     while (double_forward_mb) {
	int pawn_position = bithack.leastSignificant(double_forward_mb);
	uint64_t pawn = 1ULL << pawn_position;
	int from = pawn_position + 16;
	Move pawn_move = Move(from, pawn_position, PAWN, -1);
	next_moves->setMove(pawn_move);
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
	int taken_position = detectTakenPiece(pawn);
	Move pawn_move = Move(from, pawn_position, PAWN, taken_position);
	next_moves->setMove(pawn_move);
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
	int taken_position = detectTakenPiece(pawn);
	Move pawn_move = Move(from, pawn_position, PAWN, taken_position);
	next_moves->setMove(pawn_move);
	least_significant_take_mb &= ~pawn;
    }
 
}




void Board::initializeFromFen(Fen fen_obj){
    std::ostringstream str_stream;
    str_stream << fen_obj;
    string fen = str_stream.str();
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
    if (c == 'w'){
	half_turn = WHITE;
    } else {
	half_turn = BLACK;
    }


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
	    errFile << "Queen move direction is not valid: " << from << 
		" to " << to << "\n";
	    returnBool = false;
	}

	    
    }
    else if (move.type == ROOK){
	if (dx != 0 && dy != 0){
	    errFile << "Rook move direction is not valid: " << from 
		<< " to " << to << "\n";
	    returnBool = false;
	}
    }
    else if (move.type == BISHOP){
	if (dx != dy){
	    errFile << "Bishop move direction is not valid: " << from 
		<< " to " << to << "\n";
	    returnBool = false;
	}

    }
    else if (move.type == KNIGHT){
	if (dx == 0 || dy == 0 || abs(dx) + abs(dy) != 3){
	    errFile << "Knight move is not valid: " << from 
		<< " to " << to << "\n";
	    returnBool = false;
	}

    }
    else if (move.type == PAWN){
	if (!(
	    (dx == 0 && dy == 1) || 
	    (dx == 1 && dy == 1) || 
	    (dx == 0 && dy == 2))){

	    errFile << "Pawn move is not valid: " << from 
		<< " to " << to << "\n";
	    returnBool = false;
	}
	if (abs(dx) + abs(dy) == 2 && !(to_board && opposite_pieces)){
	    errFile << "Pawn is taking when no opponent piece on square: " 
		<< from << " to " << to << "\n";
	    returnBool = false;
	}
	if ((dx == 0 && dy == 1) && 
	    to_board & (opposite_pieces & friendly_pieces)){
	    errFile << "Pawn is moving one space when blocked: " << from 
		<< " to " << to << "\n";
	    returnBool = false;

	}
	if ((dx == 0 && dy == 2) && 
	    to_board & (opposite_pieces & friendly_pieces)){
	    errFile << "Pawn is moving two spaces when blocked: " << from 
		<< " to " << to << "\n";
	    returnBool = false;
	}	

    }

    
    if (!returnBool){	
	errFile << this;
	string move_string = move.moveCode();	
	errFile << "From: " << from << " to: " << to << "\n";
	errFile << "Move: " << move_string << "\n\n";
    }
    return returnBool;



}




void Board::debug(){
    uint64_t piece = 0x8000000000000000;

}

