#include <iostream>
#include <emscripten.h>
#include "engine/Search.h"


// "K7/8/3Q4/8/8/3q4/8/k7 w - - 0 1"

extern "C" {


EMSCRIPTEN_KEEPALIVE
const char* engineMove(const char* inp_fen){
    string fen_str = inp_fen;
    //Fen fen(fen_str);
    Board *board = new Board((Fen(fen_str)));
    Move* engineMove = new Move();
    min(board, 4, engineMove);
    board->makeMove(*engineMove);

    string output_str = board->fen();
    free(board);

    char* output;
    output = (char*)malloc(sizeof *output * (output_str.length() + 1));
    strcpy(output, output_str.c_str()); 


    /* char* output;
    output = (char*)malloc(sizeof *output * std::strlen(inp_fen));
    strcpy(output, inp_fen);  */


    return output;
}


EMSCRIPTEN_KEEPALIVE
void freeStr(const char* fen){
    free((void*)fen);    
}


}
