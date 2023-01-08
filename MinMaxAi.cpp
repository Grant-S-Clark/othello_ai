#include "MinMaxAi.h"

/*
  I will be using the move {-1, -1} frequently to denote failure
  to find moves, and get_move will return {-1, -1} if there is no
  available move to make.
*/

namespace min_max
{
    bool terminal_test(const Board & b)
    {
        std::pair< unsigned int, unsigned int > score = b.score();
        if (score.first + score.second == b.size() * b.size())
            return true;
        return false;
    }

    RowColPair available_corner(Board & state, const bool is_black)
    {
        std::unordered_set< RowColPair > moves = state.get_valid_moves(is_black);

        for (const RowColPair & rc : moves)
            if (state.is_corner(rc))
                return rc;

        return {-1, -1};
    }


    RowColPair blocking_move(Board & state, const bool is_black)
    {
        std::unordered_set< RowColPair > moves = state.get_valid_moves(is_black);
        for (const RowColPair & rc : moves)
        {
            Board result = state.result(is_black, rc);

            if (result.get_valid_moves(!is_black).size() == 0)
                return rc;
        }
    
        return {-1, -1};
    }


    std::pair< RowColPair, int > best_move(
        Board state,
        const bool is_black,
        Timer & timer,
        const int time_left,
        const int max_depth,
        const int alpha,
        const int beta,
        const int heuristic_val,
        const RowColPair move
        )
    {
        // Base cases
        if (terminal_test(state) || max_depth == 0 || time_left <= 0)
            return { move, heuristic_val };

        std::unordered_set< RowColPair > moves = state.get_valid_moves(is_black);
        int ret_heuristic  = (is_black ? INT_MAX : INT_MIN);
        RowColPair ret_move = {-1, -1};

        if (moves.size() == 0)
            return {move, -ret_heuristic};

        if (is_black)
        {
            for (const RowColPair & rc : moves)
            {
                Board b(state.result(is_black, rc));
                std::pair< RowColPair, int > p = best_move(state.result(is_black, rc),
                                                           !is_black,
                                                           timer,
                                                           time_left - (int)((timer.gettime() * 1000) - timer.starttime()),
                                                           max_depth - 1,
                                                           ret_heuristic,
                                                           beta,
                                                           state.heuristic(is_black, rc),
                                                           rc);
                if (p.second < ret_heuristic)
                {
                    ret_heuristic = p.second;
                    ret_move = rc;

                    // No need to continue checking.
                    if (ret_heuristic < beta)
                        break;
                }
            }
        }

        else
        {
            for (const RowColPair & rc : moves)
            {
                Board b(state.result(is_black, rc));
                std::pair< RowColPair, int > p = best_move(state.result(is_black, rc),
                                                           !is_black,
                                                           timer,
                                                           time_left - (int)((timer.gettime() * 1000) - timer.starttime()),
                                                           max_depth - 1,
                                                           alpha,
                                                           ret_heuristic,
                                                           state.heuristic(is_black, rc),
                                                           rc);
                if (p.second > ret_heuristic)
                {
                    ret_heuristic = p.second;
                    ret_move = rc;

                    // No need to continue checking.
                    if (ret_heuristic > beta)
                        break;
                }
            }
        }

        return { ret_move, ret_heuristic };
    }
                     

    RowColPair get_move(const int board_size,
                        const char * board,
                        const bool is_black,
                        const int time_left,
                        const int opponent_time_left)
    {
        Board state(board_size, board);

        std::pair< unsigned int, unsigned int > score = state.score();

        unsigned int b = score.first, w = score.second;

        //No moves have been made, make a random move.
        if (w + b == 4)
        {
            std::unordered_set< RowColPair > moves = state.get_valid_moves(is_black);

            int i = rand() % moves.size();

            std::unordered_set< RowColPair >::iterator it = moves.begin();
            std::advance(it, i);
        
            return *it;
        }

    
    
        //Check for corners
        RowColPair ret = available_corner(state, is_black);

        if (ret.r != -1)
            return ret;

        //Check for blocking moves
        ret = blocking_move(state, is_black);

        if (ret.r != -1)
            return ret;

        unsigned int moves_left = (state.size() * state.size()) - (w + b);
        int turn_time;

        // No division by zero.
        if (moves_left > 2)
            turn_time = time_left / (moves_left / 2);
        else
            turn_time = time_left;

        // Give more time to think in the second half of the game.
        if (moves_left > (state.size() * state.size() - 4) / 2)
            turn_time -= 1000;
        else
            turn_time -= 250;

        // Probably need to do tests to see what max depth is optimal for each size.
        int max_depth;
        if (state.size() <= 6)
            max_depth = 10;
        else if (turn_time < 1000)
            max_depth = 4;
        else
            max_depth = state.size() + 2;

        // Perform alpha-beta pruning
        Timer timer;
        timer.start();

        ret = best_move(state,
                        is_black,
                        timer,
                        turn_time,
                        max_depth).first;

        return ret;
    }
    
}
