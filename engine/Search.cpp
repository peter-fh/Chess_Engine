#include <limits.h>
#include <algorithm>
#include "Search.h"

int leaf_positions;

Move* alphaBetaMinimaxRoot(Board* board, int depth){
    if (depth <= 0){
        return board->getMoves()->randomMove();
    }
    bool maximizingPlayer = board->turn() % 2 == 0;
    int alpha = std::numeric_limits<int>::min();
    int beta = std::numeric_limits<int>::max();
    Move* engineMove = new Move();
    leaf_positions = 0;

    if (maximizingPlayer){
        int max_eval = std::numeric_limits<int>::min();
        Moves* leaf_moves = board->getMoves();
        Move* move = leaf_moves->getMove();
        *engineMove = *move;
        while (!move->isNull()){
            board->makeMove(move);
            int eval = alphaBetaMinimax(board, depth-1, alpha, beta, false); 
            std::cerr << *move << " " << eval << "\n";
            board->unmakeMove(move);
            if (eval > max_eval){
                max_eval = eval;
                *engineMove = *move;
            }
            alpha = std::max(alpha, eval);
            if (beta <= alpha){
                break;
            }
            leaf_moves->next();
            move = leaf_moves->getMove();
        }
        return engineMove;
    }

    int min_eval = std::numeric_limits<int>::max();
    Moves* leaf_moves = board->getMoves();
    Move* move = leaf_moves->getMove();
    *engineMove = *move;
    while (!move->isNull()){
        board->makeMove(move);
        int eval = alphaBetaMinimax(board, depth-1, alpha, beta, true);
        std::cerr << *move << " " << eval << "\n";
        board->unmakeMove(move);
        if (eval < min_eval){
            min_eval = eval;
            *engineMove = *move;
        }
        beta = std::min(beta, eval);
        if (beta <= alpha){
            break;
        }
        // std::cerr << "Board after unmaking " << *move << ":\n" << *board << std::endl;
        leaf_moves->next();
        move = leaf_moves->getMove();
    }
    // std::cerr << "Engine choice: " << *engineMove << "\n";
    // std::cerr << "Board inside alphaBetaMaxRoot: " << *board << "\n";
    std::cerr << "leaf positions: " << leaf_positions << "\n";
    return engineMove;
}


int alphaBetaMinimax(Board* board, int depth, int alpha, int beta, bool maximizingPlayer){
    if (depth == 0 || board->gameOver()){
        leaf_positions++;
        return board->evaluate();
    }

    if (maximizingPlayer){
        int max_eval = std::numeric_limits<int>::min();
        Moves* leaf_moves = board->getMoves();
        Move* move = leaf_moves->getMove();
        while (!move->isNull()){
            board->makeMove(move);
            int eval = alphaBetaMinimax(board, depth-1, alpha, beta, false); 
            board->unmakeMove(move);
            max_eval= std::max(max_eval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha){
                break;
            }
            delete move;
            leaf_moves->next();
            move = leaf_moves->getMove();
        }
        delete leaf_moves;
        return max_eval;
    }

    int min_eval = std::numeric_limits<int>::max();
    Moves* leaf_moves = board->getMoves();
    Move* move = leaf_moves->getMove();
    while (!move->isNull()){
        board->makeMove(move);
        int eval = alphaBetaMinimax(board, depth-1, alpha, beta, true);
        board->unmakeMove(move);
        min_eval = std::min(min_eval, eval);
        beta = std::min(beta, eval);
        if (beta <= alpha){
            break;
        }
        delete move;
        leaf_moves->next();
        move = leaf_moves->getMove();
    }
    delete leaf_moves;
    return min_eval;
}

Move* negaMaxRoot(Board* board, int depth){
    Move* bestMove = new Move(); 
    int value = std::numeric_limits<int>::min();
    Moves* leaf_moves = board->getMoves();
    //std::cerr << leaf_moves;
    Move* move = leaf_moves->getMove();
    while (!move->isNull()){
        board->makeMove(move);
        int next_value = -negaMax(board, depth-1);
        // std::cerr << *move << " " << next_value << "\n";
        if (next_value > value){
            bestMove = move; 
            value = next_value;
        }
        board->unmakeMove(move);
        leaf_moves->next();
        move = leaf_moves->getMove();
    }
    free(leaf_moves);

    //std::cerr << "best move at depth " << depth << ": " << *bestMove << "\n";

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
    Move* move = leaf_moves->getMove();
    while (!move->isNull()){
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

    //std::cerr << "best move at depth " << depth << ": " << *bestMove << "\n";

    return value;
}
