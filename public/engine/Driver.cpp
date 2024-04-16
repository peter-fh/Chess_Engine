#include <iostream>
#include "Engine.h"


using std::cout;
using std::cin;

class Driver {
public:
    static void run(){
	string inp_fen;
	getline(cin, inp_fen);
	Board board(inp_fen);
	Move* engineMove = new Move();
	min(&board, 4, engineMove);
	board.makeMove(*engineMove);
	cout << board.fen();

    }
};

// "K7/8/3Q4/8/8/3q4/8/k7 w - - 0 1"
int main(){
    Driver::run();
    
    return 0;
}
