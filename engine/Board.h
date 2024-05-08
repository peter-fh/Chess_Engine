#include <cstdint>
#include <map>
#include <stdint.h>
#include <stdbool.h>
#include <string>
#include "Bithack.h"
#include "Moves.h"
#include "Fen.h"
#include "PST.h"


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
    void makeMove(Move move);
    void unmakeMove(Move move);

    int turn();
    string fen();
    friend ostream &operator<<(ostream& out, const Board &board);
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

    int evaluatePiece(uint64_t board, int* table, int evaluation);
    void initializeSidedBoard();
public: 
    void debug();

};


const int WHITE = 0;
const int BLACK = 1;



typedef uint64_t (Board::*pieceMoves)(int);
