#include <iostream>
#include "Search.h"
using std::cerr;

Move* minimax(Board *board, int depth){
    if (board->turn() % 2 == 0) {
	cerr << "starting with max\n";
	return max_root(board, depth);
    } 
    cerr << "starting with min\n";
    return min_root(board, depth);
}

Move* max_root(Board *board, int depth){
    int max_eval = -1000000;
    Move* move = new Move();

    Moves moves = board->getMoves();

    while (moves.hasNext()){
	Move nextMove = moves.getMove();
	board->makeMove(nextMove);

	int eval = min(board, depth-1);
	if (eval > max_eval){
	    max_eval = eval;
	    *move = nextMove;
	}

	board->unmakeMove(nextMove);
	moves.next();

    }
    return move;
}

Move* min_root(Board *board, int depth){
    int min_eval = 1000000;
    Move* move = new Move();

    Moves moves = board->getMoves();

    while (moves.hasNext()){
	Move nextMove = moves.getMove();
	board->makeMove(nextMove);
	int eval = max(board, depth-1);
	if (eval < min_eval){
	    min_eval = eval;
	    *move = nextMove;
	}

	board->unmakeMove(nextMove);
	moves.next();
    }
    return move;
}

int max(Board *board, int depth){
    if (depth == 0){
	return board->evaluate();
    }

    int max_eval = -1000000;

    Moves moves = board->getMoves();

    while (moves.hasNext()){
	Move nextMove = moves.getMove();
	board->makeMove(nextMove);

	int eval = min(board, depth-1);
	if (eval > max_eval){
	    max_eval = eval;
	}

	board->unmakeMove(nextMove);
	moves.next();

    }
    return max_eval;
}


int min(Board *board, int depth){
    if (depth == 0){
	return board->evaluate();
    }

    int min_eval = 1000000;

    Moves moves = board->getMoves();

    while (moves.hasNext()){
	Move nextMove = moves.getMove();
	board->makeMove(nextMove);
	int eval = max(board, depth-1);
	if (eval < min_eval){
	    min_eval = eval;
	}

	board->unmakeMove(nextMove);
	moves.next();
    }
    return min_eval;
}


