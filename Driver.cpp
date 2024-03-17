#include <iostream>
#include "Board.h"


using std::cout;

void dfs(Board *chess_board, int depth){
    if (depth == 0) {
	return;
    }

   
    cout << "\nBoard (depth " << depth << "):\n";
    cout << chess_board->toString();
    Moves board_moves = chess_board->getMoves();
    board_moves.displayMoves();
    cout << "\n";
    while (board_moves.hasNext()){
	Move current_move = board_moves.getMove();
	if (!chess_board->isLegal(current_move)){
	    cout << "Illegal move found\n";
	}

	chess_board->makeMove(current_move);
	dfs(chess_board, depth - 1);
	chess_board->unmakeMove(current_move);
	board_moves.next();
    }




}

void debugMoves(int depth){
    Board board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    dfs(&board, depth);

}

void tryTwoMoves(){

    Board board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Board* chess_board = &board;
    Move *move1 = new Move(8, 16, 5, false);
    Move *move2 = new Move(48, 40, 5, false);
    Move *move3 = new Move(16, 24, 5, false);

    chess_board->makeMove(*move1);
    cout << "First move:\n";
    cout << chess_board->toString();

    chess_board->makeMove(*move2);
    cout << "Second move:\n";
    cout << chess_board->toString();

    chess_board->makeMove(*move3);
    cout << "Third move:\n";
    cout << chess_board->toString();

    chess_board->unmakeMove(*move3);
    cout << "Unmade move 3:\n"; 
    cout << chess_board->toString();

    chess_board->unmakeMove(*move2);
    cout << "Unmade move 2:\n"; 
    cout << chess_board->toString();

    chess_board->unmakeMove(*move1);
    cout << "Unmade move 1:\n"; 
    cout << chess_board->toString();
}


int main(){
    debugMoves(3);
    //tryTwoMoves();
    return 0;
}
