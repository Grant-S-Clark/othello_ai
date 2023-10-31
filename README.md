# othello_ai
A deterministic AI that plays the Othello board game written in C++.

Uses the minimax algorithm to generate the best next move in the board game Othello.
To build the program, simply build with g++ like so: "g++ *.cpp" (you must have boost installed).
It uses the boost library's dynamic_bitset to perform board computations extremely quickly, making
the moves generate very fast. The board can only be 4x4, 6x6, 8x8, and 10x10.
