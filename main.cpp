#include <iostream>
#include <bitset>
#include <ctime>
#include <cstdlib>

#include "Board.h"
#include "MinMaxAi.h"
#include "RandomAi.h"

void update_char_board(char board[], const Board & b)
{
    std::string b_str = b.board_string();

    int n = b.size() * b.size();

    for (int i = 0; i < n; ++i)
        if (b_str[(n - 1) - i] == '1')
            board[i] = 'B';

    for (int i = 0; i < n; ++i)
        if (b_str[(2 * n - 1) - i] == '1')
            board[i] = 'W';

    return;
}

int main()
{
    srand((unsigned int)time(NULL));

    int size = 0;
    while (size != 6 && size != 8 && size != 10)
    {
        std::cout << "Input Size (6, 8, 10): ";
        std::cin >> size;
    }

    char * board = new char[size * size];
    for (unsigned int i = 0; i < size * size; ++i)
        board[i] = ' ';

    board[(size / 2) * size + (size / 2)] = 'B';
    board[(size / 2 - 1) * size + (size / 2 - 1)] = 'B';
    
    board[(size / 2 - 1) * size + (size / 2)] = 'W';
    board[(size / 2) * size + (size / 2 - 1)] = 'W';
    
    Board b(size, board);
    
    char color;
    bool is_black = true;
    
    std::cout << "MinMax Color (w or b): ";
    std::cin >> color;

    RowColPair (*black_get_move)(const int, const char *, const bool, const int, const int);
    RowColPair (*white_get_move)(const int, const char *, const bool, const int, const int);

    if (color == 'b' || color == 'B')
    {
        black_get_move = min_max::get_move;
        white_get_move = random_ai::get_move;
    }
    else
    {
        white_get_move = min_max::get_move;
        black_get_move = random_ai::get_move;
    }

    int black_time = 150000, white_time = 150000;

    bool player_blocked = false;

    Timer timer;

    std::cout << b;
    
    while(1)
    {
        // Print info.
        std::pair< unsigned int, unsigned int > score = b.score();
        std::cout << "B:" << score.first << ", W:" << score.second << ", WINNER:";
        if (score.first > score.second)
            std::cout << "B";
        else if (score.first < score.second)
            std::cout << "W";
        else
            std::cout << "TIE";
        std::cout << std::endl;

        if (score.first + score.second == size * size)
        {
            std::cout << "ALL PIECES PLACED, GAME OVER." << std::endl;
            break;
        }
        
        std::cout << std::endl;
        if (is_black)
            std::cout << "Black's Turn..." << std::endl;
        else
            std::cout << "White's Turn..." << std::endl;
        std::cout << std::endl;

        RowColPair move;

        timer.start();
        if (is_black)
        {
            move = black_get_move(size, board, is_black, black_time, white_time);
            timer.stop();
            black_time -= (timer.read() * 1000);

            if (black_time <= 0)
            {
                std::cout << "BLACK TIMED OUT, WHITE WINS." << std::endl;
                break;
            }
        }
        
        else
        {
            move = white_get_move(size, board, is_black, white_time, black_time);
            timer.stop();
            white_time -= (timer.read() * 1000);

            if (white_time <= 0)
            {
                std::cout << "WHITE TIMED OUT, BLACK WINS." << std::endl;
                break;
            }
        }

        // Not Blocked.
        if (move.r != -1)
        {
            b = b.result(is_black, move);
            update_char_board(board, b);
            player_blocked = false;
        }

        else
        {
            if (is_black)
                std::cout << "BLACK IS BLOCKED." << std::endl;
            else
                std::cout << "WHITE IS BLOCKED." << std::endl;
            
            if (player_blocked)
            {
                std::cout << "BOTH PLAYERS BLOCKED. GAME OVER." << std::endl;
                break;
            }
            
            player_blocked = true;
        }
        
        is_black = !is_black;

        std::cout << b;
    }
    
    return 0;
}
