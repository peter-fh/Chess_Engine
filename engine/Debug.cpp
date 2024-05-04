#include <iostream>
#include "Search.h"

using std::cout;
class Debug {
public:
    int passes = 0;

    void dfs(Board *chess_board, int depth){

	if (depth == 0) {
	    passes++;
	    //cout << chess_board->toString();
	    return;
	}

	Moves board_moves = chess_board->getMoves();
	while (board_moves.hasNext()){

	    Move current_move = board_moves.getMove();

	    chess_board->makeMove(current_move);
	    dfs(chess_board, depth - 1);
	    chess_board->unmakeMove(current_move);

	    board_moves.next();
	}

    }


    void dfsDebug(Board *chess_board, int depth){

	// cout << "\nBoard (depth " << depth << "):\n";
	// cout << chess_board->toString();
	if (depth == 0) {
	    return;
	}

	Moves board_moves = chess_board->getMoves();
	// board_moves.displayMoves();
	// cout << "\n";

	//while (chess_board->validBoardState() && board_moves.hasNext()){
	while (board_moves.hasNext()){
	    if (!chess_board->validBoardState()){
		cout << chess_board->toString();
		cout << "Invalid state. Exiting.\n";
		exit(0);
	    } 
	    Move current_move = board_moves.getMove();
	    if (!chess_board->isLegal(current_move)){
		// cout << "Illegal move found\n";
	    } 

	    chess_board->makeMove(current_move);
	    // cout << current_move.moveCode();
	    dfsDebug(chess_board, depth - 1);
	    chess_board->unmakeMove(current_move);
	    board_moves.next();
	}

    }

    void playGame(){
	Board board = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	Board *chess_board = &board;
	Moves board_moves = chess_board->getMoves();
	while (board_moves.length() != 0 ){
	    // cout << "BEFORE MOVES:\n";
	    cout << chess_board->toString();
	    if (!chess_board->validBoardState()){
		cout << "Invalid state. Exiting.\n";
		exit(0);
	    }
	    Move random_move = board_moves.randomMove();
	    chess_board->makeMove(random_move);
	    // cout << "AFTER 1 MOVE:\n";
	    // cout << chess_board->toString();
	    // chess_board->unmakeMove(random_move);
	    // cout << "AFTER UNDO MOVE:\n";
	    // cout << chess_board->toString();
	    // chess_board->makeMove(random_move);
	    // cout << "AFTER 1 MOVE:\n";
	    // cout << chess_board->toString();

	    board_moves = chess_board->getMoves();
	}
    }
    void debugMoves(int depth){
	Board board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	dfs(&board, depth);
	if (!board.validBoardState()){
	    cout << "Invalid board state!\n";
	}

    }


    static void tryTwoMoves(){

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

    static void debugFen(){
	string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	Board board(fen);
	cout << "Fen before: " << fen << "\n";
	cout << "Fen after:  " << board.fen() << "\n";
    }
};


int main(){
    Debug::debugFen();
}
