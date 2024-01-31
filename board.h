#include <cstdint>
#include <map>
#include <stdint.h>
#include <stdbool.h>
#include <string>
#include "bithack.h"

using namespace std;


extern Bithack bithack;


const int POSITIVE = 1;
const int NEGATIVE = -1;

// Bitshift left by this amount travels one square in this direction:
const int WEST = 1;
const int NORTHWEST = 9;
const int NORTH = 8;
const int NORTHEAST = 7;
const int EAST = -1;
const int SOUTHEAST = -9;
const int SOUTH = -8;
const int SOUTHWEST = -7;






const int FROM = 0;
const int TO = 1;

class Move{
    public:
        int squares[2];
        int type;
        bool take;

    Move(): type(-1) {}

    Move(int from, int to, int inptype, bool inptake) {
        squares[FROM] = from; 
        squares[TO]= to;
        type = inptype;
        take = inptake;    
    }

    string moveCode();
    bool isNull();
    bool equals(Move move);
};


class Moves{
    Move moves[150];
    int index;
    int len;


    public:

        Moves(): index(0), len(0){} 
        Move getMove();
        void next();
        bool seek(int seekIndex);
        bool hasNext();
        bool isNull();
        void setMove(Move move);
        void setLast();
        int length();
        void displayMoves();
        void processMoveBoard(uint64_t move_board, uint64_t other_pieces, int piece_position, int piece_type);
        

};


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
    


    uint64_t directionalMoves(int position, int direction, uint64_t all_pieces, uint64_t other_pieces);
    uint64_t straightMoves(int position, uint64_t all_piees, uint64_t other_pieces);
    uint64_t diagonalMoves (int position, uint64_t all_pieces, uint64_t other_pieces);    
    

    public:
        void display_bitboard(uint64_t board);
        void initializeFromFen(string fen);
        Board(){
        }
        Board(string fen){
            initializeFromFen(fen);

        }
        uint64_t getKnightMoves(int position, uint64_t same_pieces);
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



