#ifndef MIN_MAX_AI_H
#define MIN_MAX_AI_H

#include <limits.h>

#include "Board.h"
#include "Timer.h"

namespace min_max
{
    bool terminal_test(const Board & b);
    RowColPair available_corner(Board & state, const bool is_black);
    RowColPair blocking_move(Board & state, const bool is_black);

    std::pair< RowColPair, int > best_move(Board state,
                                           const bool is_black,
                                           Timer & timer,
                                           const int time_left,
                                           const int max_depth = 4,
                                           const int alpha = INT_MAX,
                                           const int beta = INT_MIN,
                                           const int heuristic_val = INT_MAX,
                                           const RowColPair move = {-1, -1});
    
    RowColPair get_move(const int board_size,
                        const char * board,
                        const bool is_black,
                        const int time_left,
                        const int opponent_time_left);
}

#endif
