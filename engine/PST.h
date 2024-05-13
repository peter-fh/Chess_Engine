#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
using std::string;


const string filenames[12] = {"wk", "wq", "wr", "wb", "wn", "wp","bk", "bq", "br", "bb", "bn", "bp",};

class PST {
public:
    PST();
    uint64_t tables[12][64];
private:
     

    bool readPST(string piece, uint64_t pst[64]);
    friend std::ostream& operator<<(std::ostream& out, const PST pst);
};
