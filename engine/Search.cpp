#include <limits.h>
#include <algorithm>
#include "Search.h"
using std::cerr;


Move* negaMaxRoot(Board* board, int depth){
    Move* bestMove = new Move(); 
    int value = std::numeric_limits<int>::min();
    Moves* leaf_moves = board->getMoves();
    //cerr << leaf_moves;
    Move move = leaf_moves->getMove();
    while (!move.isNull()){
        board->makeMove(move);
        int next_value = -negaMax(board, depth-1);
        cerr << move << " " << next_value << "\n";
        if (next_value > value){
            *bestMove = move; 
            value = next_value;
        }
        board->unmakeMove(move);
        leaf_moves->next();
        move = leaf_moves->getMove();
    }
    free(leaf_moves);

    //cerr << "best move at depth " << depth << ": " << *bestMove << "\n";

    return bestMove;
}


int negaMax(Board* board, int depth){
    if (depth == 0){
        if (board->turn() % 2 == 0){
            return board->evaluate();
        }
        return -board->evaluate();
    }

    int value = std::numeric_limits<int>::min();
    Moves* leaf_moves = board->getMoves();
    Move move = leaf_moves->getMove();
    while (!move.isNull()){
        board->makeMove(move);
        int next_value = -negaMax(board, depth-1);
        if (next_value > value){
            value = next_value;
        }
        board->unmakeMove(move);
        leaf_moves->next();
        move = leaf_moves->getMove();
    }
    free(leaf_moves);

    //cerr << "best move at depth " << depth << ": " << *bestMove << "\n";

    return value;
}
