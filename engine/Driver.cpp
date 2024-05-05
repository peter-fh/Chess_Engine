#include "Search.h"
using std::cout;

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


// TODO: debug waiting for a while and returning nothing 1/5 times
int main(int argc, char* argv[]){
    if (argc != 2){
        cout << "Invalid call to Driver.cpp (requires http encoded fen)\n";
        return 1;
    }
    string http_fen = argv[1];
    string fen = decode_fen(http_fen);
    Board *board = new Board(Fen(fen));
    Move *engine_move = new Move();
    max(board, 4, engine_move);
    board->makeMove(*engine_move);
    cout << board->fen();
    free(board);
    free(engine_move);

    return 0;
}
