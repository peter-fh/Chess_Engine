#include "Board.h"

// TODO: alpha beta pruning
// TODO: quiescent search

Move* negaMaxRoot(Board* board, int depth);
int negaMax(Board* board, int depth);
Move* minimax_root(Board* board, int depth, int turn);
int minimax(Board* board, int depth, bool maximizingPlayer);
