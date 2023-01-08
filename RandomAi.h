#ifndef RANDOM_AI
#define RANDOM_AI

#include "Board.h"

namespace random_ai
{
    RowColPair get_move(const int board_size,
                        const char * board,
                        const bool is_black,
                        const int time_left,
                        const int opponent_time_left);
}

#endif
