#include "PST.h"
using std::cerr;


PST::PST(){
    for (int i=0; i < 12; i++){
        readPST(filenames[i], tables[i]);
    }
}


bool PST::readPST(string piece, uint64_t pst[64]){
    string filename = "engine/PST/" + piece + ".txt";
    std::ifstream in(filename);
    if (!in.is_open()){
        cerr << "could not read PST: " << filename << "\n";
        return false;
    };
    string line;
    for (int i=0; i < 64; i++){
        std::getline(in, line, ',');
        pst[i] = std::stoi(line);
    }

    return true;

}


std::ostream& operator<<(std::ostream& out, const PST pst){
    for (int i=0; i < 12; i++){
        for (int j=0; j < 64; j++){
            out << pst.tables[i][j];
        }
    }
    return out;
}
