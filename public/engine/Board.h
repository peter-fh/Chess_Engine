#include <cstdint>
#include <map>
#include <stdint.h>
#include <stdbool.h>
#include <string>
#include "Bithack.h"
#include "Moves.h"


class Board{

public:

    Board(string fen);
    Board(Board *inp_board);

    Moves getMoves();
    void makeMove(Move move);
    void unmakeMove(Move move);

    int turn();
    string fen();
    string toString();
    string bitboardToString(uint64_t board);
    bool isLegal(Move move);
    bool validBoardState();
    int evaluate();

private:
    uint64_t pieces[12];
    bool castling[4];
    int phantom_pawn;
    int half_turn;
    Move previous_move;
    Bithack bithack; 
    Moves legal_moves;
    int previous_taken_piece;

    void initializeFromFen(string fen);
    uint64_t directionalMoves(int position, int direction, uint64_t all_pieces, uint64_t other_pieces);
    uint64_t straightMoves(int position, uint64_t all_piees, uint64_t other_pieces);
    uint64_t diagonalMoves (int position, uint64_t all_pieces, uint64_t other_pieces);    
    bool lonePiece(uint64_t piece);
    uint64_t knightMoves(int position, uint64_t same_pieces, uint64_t other_pieces);
    uint64_t getKingMoves(int position, uint64_t same_pieces);
    void whitePawnMoves(uint64_t pawns, uint64_t all_pieces, uint64_t other_pieces, Moves *moves);
    void blackPawnMoves(uint64_t pawns, uint64_t all_pieces, uint64_t other_pieces, Moves *moves);
    int detectTakenPiece(uint64_t move_board);
    void processDoublePieceMoves(uint64_t piece, uint64_t all_pieces, uint64_t other_pieces, Moves *moves, uint64_t (Board::*getPieceMoves) (int, uint64_t, uint64_t), int piece_type);
    void processMoveBoard(Moves *moves, uint64_t move_board, uint64_t other_pieces, int piece_position, int piece_type);

public: 
    void debug();

};


const int WHITE = 0;
const int BLACK = 1;


typedef uint64_t (Board::*pieceMoves)(int, uint64_t, uint64_t);
