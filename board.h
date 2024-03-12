#include <cstdint>
#include <map>
#include <stdint.h>
#include <stdbool.h>
#include <string>
#include "Bithack.h"
#include "Moves.h"


const int KING = 0;
const int QUEEN = 1;
const int ROOK = 2;
const int BISHOP = 3;
const int KNIGHT = 4;
const int PAWN = 5;


class Board{

    
    uint64_t pieces[12];
    bool castling[4];
    int phantom_pawn;
    int half_turn;
    Move previous_move;
    Bithack bithack; 

    
    uint64_t directionalMoves(int position, int direction, uint64_t all_pieces, uint64_t other_pieces);
    uint64_t straightMoves(int position, uint64_t all_piees, uint64_t other_pieces);
    uint64_t diagonalMoves (int position, uint64_t all_pieces, uint64_t other_pieces);    

    void whitePawnMoves(int64_t pawns, uint64_t all_pieces, uint64_t other_pieces, Moves *moves);

    void blackPawnMoves(int64_t pawns, uint64_t all_pieces, uint64_t other_pieces, Moves *moves);
    void processDoublePieceMoves(uint64_t piece, uint64_t all_pieces, uint64_t other_pieces, Moves *moves, uint64_t (Board::*getPieceMoves) (int, uint64_t, uint64_t), int piece_type);    

    void processMoveBoard(Moves *moves, uint64_t move_board, uint64_t other_pieces, int piece_position, int piece_type);

public:
    void display_bitboard(uint64_t board);
    void initializeFromFen(string fen);
    Board();
    Board(string fen);
    uint64_t knightMoves(int position, uint64_t same_pieces, uint64_t other_pieces);
    bool lonePiece(uint64_t piece);
    Moves legal_moves;
    int turn();
    bool isLegal(Move move);
    Moves getMoves();
    void makeMove(Move move);
    void push();
    void pop();
    string fen();
    void debug();
    void printBoard();
    uint64_t getKingMoves(int position, uint64_t all_pieces, uint64_t other_pieces);
	
};



typedef uint64_t (Board::*pieceMoves)(int, uint64_t, uint64_t);
