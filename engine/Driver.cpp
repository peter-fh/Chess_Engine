#include "Search.h"
using std::cout;
//using std::cerr;

string decode_fen(string http_fen){
    std::unordered_map<char, char> decode_map;

    decode_map['_'] = '/';
    decode_map['+'] = ' ';


    string fen = "";
    for (int i=0; i < http_fen.length(); i++){
        if (decode_map.count(http_fen[i])){
            fen.push_back(decode_map[http_fen[i]]);
        } else {
            fen.push_back(http_fen[i]);
        }
    }
    return fen;
}


int main(int argc, char* argv[]){
    if (argc != 2){
        cout << "Invalid call to Driver.cpp (requires http encoded fen)\n";
        return 1;
    }
    string http_fen = argv[1];
    string fen = decode_fen(http_fen);
    Fen c_fen(fen);
    Board *board = new Board(c_fen);
    Move *engine_move = negaMaxRoot(board, 4);
    board->makeMove(*engine_move);
    cout << board->fen();
    free(board);
    free(engine_move);

    return 0;
}
