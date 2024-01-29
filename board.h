#include <stdint.h>
#include <stdbool.h>
#include <string>
using namespace std;


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
    Move moves[120];
    int index;
    int len;


    public:
        Move getMove();
        void next();
        bool seek(int seekIndex);
        bool hasNext();
        bool isNull();
        void setMove(Move move);
        void setLast();
        int length();
        

};


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



class Rays{
    uint64_t horizontal[8];
    uint64_t vertical[8];
    uint64_t diagonal[15];
    uint64_t counter_diagonal[15];


    public:
        void loadRays();
        uint64_t getHorizontal(int position);
        uint64_t getVertical(int position);
        uint64_t getDiagonal(int position);
        uint64_t getCounterDiagonal(int position);
        uint64_t directionalMask(int position, int direction);
        uint64_t castRay(int position, int direction);
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

    // Utility
    Rays rays;
    uint64_t least_significant_positions[65536];
    uint64_t most_significant_positions[65536];
    

    void initializeBitPositions();
    int slowLeastSignificant(int piece);
    int slowMostSignificant(int piece);
    int leastSignificant(uint64_t piece);
    int mostSignificant(uint64_t piece);
    uint64_t directionalMoves(int position, int direction, uint64_t all_pieces, uint64_t other_pieces);
    
    

    public:
        void display_bitboard(uint64_t board);
        Board() {
            initializeBitPositions();
            rays.loadRays();
        }
        bool loneBit(uint64_t piece);
        Moves legal_moves;
        int turn();
        bool isLegal(Move move);
        Moves determineLegalMoves();
        void makeMove(Move move);
        void push();
        void pop();
        string fen();
        void debug();
    


};



