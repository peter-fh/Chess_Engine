#include <limits.h>
#include <algorithm>
#include "Search.h"
using std::cerr;

Move* negaMaxRoot(Board* board, int depth){
    Move* bestMove = new Move(); 
    int value = std::numeric_limits<int>::min();
    Moves leaf_moves = board->getMoves();
    //cerr << leaf_moves;
    Move move = leaf_moves.getMove();
    while (!move.isNull()){
        board->makeMove(move);
        int next_value = -negaMax(board, depth-1);
        cerr << move << " " << next_value << "\n";
        if (next_value > value){
            *bestMove = move; 
            value = next_value;
        }
        board->unmakeMove(move);
        leaf_moves.next();
        move = leaf_moves.getMove();
    }

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
    Moves leaf_moves = board->getMoves();
    Move move = leaf_moves.getMove();
    while (!move.isNull()){
        board->makeMove(move);
        int next_value = -negaMax(board, depth-1);
        if (next_value > value){
            value = next_value;
        }
        board->unmakeMove(move);
        leaf_moves.next();
        move = leaf_moves.getMove();
    }

    //cerr << "best move at depth " << depth << ": " << *bestMove << "\n";

    return value;
}


Move* minimax_root(Board* board, int depth, int turn){
    if (turn % 2 == 0){
        int value = std::numeric_limits<int>::min(); 
        Move* bestMove = new Move();
        Moves leaf_moves = board->getMoves();
        while (leaf_moves.hasNext()){

            Move nextMove = leaf_moves.getMove();
            board->makeMove(nextMove);
            int next_value = minimax(board, depth-1, false);
            if (next_value > value){
                *bestMove = nextMove;
            }
            value = std::max(next_value, value);
            board->unmakeMove(nextMove);
            leaf_moves.next();
        }
        return bestMove;
    } // else black turn

    // else minimizingPlayer
    int value = std::numeric_limits<int>::max();
    Move* bestMove = new Move();
    Moves leaf_moves = board->getMoves();
    while (leaf_moves.hasNext()){

        Move nextMove = leaf_moves.getMove();
        board->makeMove(nextMove);
        int next_value = minimax(board, depth-1, true);
        if (next_value < value){
            *bestMove = nextMove; 
        }
        value = std::min(next_value, value);
        board->unmakeMove(nextMove);
        leaf_moves.next();
    }
    return bestMove;


}



int minimax(Board* board, int depth, bool maximizingPlayer){
    if (depth == 0){
        if (maximizingPlayer){
            return board->evaluate();
        }
        return -board->evaluate();
    }
    if (maximizingPlayer) {
        int value = std::numeric_limits<int>::min(); 
        Moves leaf_moves = board->getMoves();
        while (leaf_moves.hasNext()){
            Move nextMove = leaf_moves.getMove();
            board->makeMove(nextMove);
            value = std::max(value, minimax(board, depth-1, false));
            board->unmakeMove(nextMove);
            leaf_moves.next();
        }
        return value;
    }

    // else minimizingPlayer
    int value = std::numeric_limits<int>::max();
    Moves leaf_moves = board->getMoves();
    while (leaf_moves.hasNext()){
        Move nextMove = leaf_moves.getMove();
        board->makeMove(nextMove);
        value = std::min(value, minimax(board, depth-1, true));
        board->unmakeMove(nextMove);
        leaf_moves.next();
    }
    return value;
}
