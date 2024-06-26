#include <string>

using std::string;

const int FROM = 0;
const int TO = 1;

const int KING = 0;
const int QUEEN = 1;
const int ROOK = 2;
const int BISHOP = 3;
const int KNIGHT = 4;
const int PAWN = 5;


class Move{

public:
    int squares[2];
    int type;
    int take;
    int eval;

    Move();
    
    Move(int from, int to, int inptype, int inptake);
    string moveCode();
    friend std::ostream& operator<<(std::ostream& out, const Move& move);
    bool isNull() const;
    bool equals(Move move);
};


