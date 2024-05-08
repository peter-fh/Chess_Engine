#include <fstream>
#include <sstream>
#include <iostream>
using std::string;

class PST {
public:
    PST();
    int white_king[64];
    int white_queen[64];
    int white_rook[64];
    int white_bishop[64];
    int white_knight[64];
    int white_pawn[64];
    int black_king[64];
    int black_queen[64];
    int black_rook[64];
    int black_bishop[64];
    int black_knight[64];
    int black_pawn[64];
private:

    bool readPST(string piece, int pst[64]);
    friend std::ostream& operator<<(std::ostream& out, const PST pst);
};
