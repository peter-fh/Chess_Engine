#include "PST.h"
using std::cerr;


PST::PST(){
    readPST("wk", white_king);
    readPST("wq", white_queen);
    readPST("wr", white_rook);
    readPST("wb", white_bishop);
    readPST("wn", white_knight);
    readPST("wp", white_pawn);
    readPST("bk", black_king);
    readPST("bq", black_queen);
    readPST("br", black_rook);
    readPST("bb", black_bishop);
    readPST("bn", black_knight);
    readPST("bp", black_pawn);
}


bool PST::readPST(string piece, int pst[64]){
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
    out << "white_king\n";
    for (int i=0; i < 64; i++){
        if (i % 8 ==0) out << "\n";
        out << pst.white_king[i] << " ";
    }
    out << "white_queen\n";
    for (int i=0; i < 64; i++){
        if (i % 8 ==0) out << "\n";
        out << pst.white_queen[i] << ",";
    }
    out << "\n";
    for (int i=0; i < 64; i++){
        if (i % 8 ==0) out << "\n";
        out << pst.white_rook[i] << ",";
    }
    out << "\n";
    for (int i=0; i < 64; i++){
        if (i % 8 ==0) out << "\n";
        out << pst.white_bishop[i] << ",";
    }
    out << "\n";
    for (int i=0; i < 64; i++){
        if (i % 8 ==0) out << "\n";
        out << pst.white_knight[i] << ",";
    }
    out << "\n";
    for (int i=0; i < 64; i++){
        if (i % 8 ==0) out << "\n";
        out << pst.white_pawn[i] << ",";
    }
    out << "\n";
    for (int i=0; i < 64; i++){
        if (i % 8 ==0) out << "\n";
        out << pst.black_king[i] << ",";
    }
    out << "\n";
    for (int i=0; i < 64; i++){
        if (i % 8 ==0) out << "\n";
        out << pst.black_queen[i] << ",";
    }
    out << "\n";
    for (int i=0; i < 64; i++){
        if (i % 8 ==0) out << "\n";
        out << pst.black_rook[i] << ",";
    }
    out << "\n";
    for (int i=0; i < 64; i++){
        if (i % 8 ==0) out << "\n";
        out << pst.black_bishop[i] << ",";
    }
    out << "\n";
    for (int i=0; i < 64; i++){
        if (i % 8 ==0) out << "\n";
        out << pst.black_knight[i] << ",";
    }
    out << "\n";
    for (int i=0; i < 64; i++){
        if (i % 8 ==0) out << "\n";
        out << pst.black_pawn[i] << ",";
    }
    out << "\n";
    return out;
}
