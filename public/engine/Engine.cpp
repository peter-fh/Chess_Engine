#include <iostream>
#include "Engine.h"


int max(Board *board, int depth, Move* move){
    if (depth == 0){
	return board->evaluate();
    }

    int max_eval = -1000000;
    Move max_move;

    Moves moves = board->getMoves();

    while (moves.hasNext()){
	Move nextMove = moves.getMove();
	board->makeMove(nextMove);

	int eval = min(board, depth-1, move);
	if (eval > max_eval){
	    max_eval = eval;
	    max_move = nextMove;
	}

	board->unmakeMove(nextMove);
	moves.next();

    }
    *move = max_move;
    return max_eval;
}


int min(Board *board, int depth, Move* move){
    if (depth == 0){
	return -board->evaluate();
    }

    int min_eval = 1000000;
    Move min_move;

    Moves moves = board->getMoves();

    while (moves.hasNext()){
	Move nextMove = moves.getMove();
	board->makeMove(nextMove);
	int eval = max(board, depth-1, move);
	if (eval < min_eval){
	    min_eval = eval;
	    min_move = nextMove;
	}

	board->unmakeMove(nextMove);
	moves.next();
    }
    *move = min_move;
    return min_eval;
}


