#include <string>

using std::string;

const int FROM = 0;
const int TO = 1;

class Move{

public:
    int squares[2];
    int type;
    bool take;

    Move();
    
    Move(int from, int to, int inptype, bool inptake);
    string moveCode();
    bool isNull();
    bool equals(Move move);
};


