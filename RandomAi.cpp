#include "RandomAi.h"

namespace random_ai
{
    RowColPair get_move(const int board_size,
                        const char * board,
                        const bool is_black,
                        const int time_left,
                        const int opponent_time_left)
    {
        Board state(board_size, board);
        std::cout << state;
        std::unordered_set< RowColPair > moves = state.get_valid_moves(is_black);

        if (moves.size() == 0)
            return {-1, -1};
        
        int i = rand() % moves.size();

        std::unordered_set< RowColPair >::iterator it = moves.begin();
        std::advance(it, i);
        
        return *it;
    }
    
}
