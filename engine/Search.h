#include "Board.h"

// TODO: quiescent search

Move* alphaBetaMinimaxRoot(Board* board, int depth);
int alphaBetaMinimax(Board* board, int depth, int alpha, int beta, bool maximizingPlayer);
Move* negaMaxRoot(Board* board, int depth);
int negaMax(Board* board, int depth);
Move* minimax_root(Board* board, int depth, int turn);
int minimax(Board* board, int depth, bool maximizingPlayer);
