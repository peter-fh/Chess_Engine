
#include <iostream>

using std::string;
using std::istream;
using std::ostream;

class Fen {
public:

    Fen();
    Fen(string fen);
    friend ostream &operator<<(ostream &output, const Fen &F);
    friend istream &operator>>(istream &input, Fen &F);

private:

    string pieces;
    string turn;
    string castle;
    string pawn;
    string halfmove;
    string fullmove;
};
