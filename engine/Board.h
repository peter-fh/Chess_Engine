#include <cstdint>
#include <map>
#include <stdint.h>
#include <stdbool.h>
#include <string>
#include "Bithack.h"
#include "Moves.h"
#include "Fen.h"
#include "PST.h"


const int KING_EVAL = 20000;
const int QUEEN_EVAL = 800;
const int ROOK_EVAL = 500;
const int BISHOP_EVAL = 315;
const int KNIGHT_EVAL = 300;
const int PAWN_EVAL = 100;
const int evals[6] = {KING_EVAL, QUEEN_EVAL, ROOK_EVAL, BISHOP_EVAL, KNIGHT_EVAL, PAWN_EVAL};

// TODO: destructor

typedef struct SidedBoard{
    uint64_t same_pieces;
    uint64_t other_pieces;
    uint64_t all_pieces;
} SidedBoard;

class Board{

public:

    Board(Fen fen);
    Board(Board *inp_board);

    Moves* getMoves();
    void makeMove(Move* move);
    void unmakeMove(Move* move);

    int turn();
    string fen();
    friend ostream &operator<<(ostream& out, const Board &board);
    string bitboardToString(uint64_t board);
    bool isLegal(Move move);
    bool validBoardState();
    int evaluate();
    bool gameOver();

private:
    uint64_t pieces[12];
    bool castling[4];
    int phantom_pawn;
    int half_turn;
    Move previous_move;
    Bithack bithack; 
    Moves legal_moves;
    int previous_taken_piece;
    PST pst;
    SidedBoard sided_board;
    Moves* next_moves;

    void initializeFromFen(Fen fen);
    void initializePST();
    void readPST(istream& in, int pst[64]);
    uint64_t directionalMoves(
        int position, int direction);
    uint64_t straightMoves(int position);
    uint64_t diagonalMoves(int position);    
    bool lonePiece(uint64_t piece);
    uint64_t knightMoves(
        int position);
    uint64_t getKingMoves(int position);
    void whitePawnMoves(uint64_t pawns);
    void blackPawnMoves(uint64_t pawns);
    int detectTakenPiece(uint64_t move_board);
    void processDoublePieceMoves(uint64_t piece,int piece_type,
        uint64_t (Board::*getPieceMoves) (int));
    void processMoveBoard(uint64_t move_board,  int piece_position, int piece_type);

    int evaluatePiece(uint64_t board, uint64_t* table, int evaluation);
    void initializeSidedBoard();
    void evaluateMove(Move* move);
    void evaluateMoves(Moves* moves);
    void sortMoves(Moves* moves);
public: 
    void debug();

};


const int WHITE = 0;
const int BLACK = 1;



typedef uint64_t (Board::*pieceMoves)(int);
