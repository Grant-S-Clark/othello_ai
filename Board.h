#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <unordered_set>
#include <unordered_map>
#include <boost/dynamic_bitset.hpp>

#include "Common.h"
#include "MoveGenUtil.h"


struct BoardData
{
    boost::dynamic_bitset< uint8_t > black;
    boost::dynamic_bitset< uint8_t > white;
    unsigned int size;
};

// std::pair hashing
namespace std
{
    // to_string
    template< typename T1, typename T2 >
    std::string to_string(const std::pair< T1, T2 > & p)
    {
        return "(" + std::to_string(p.first) + "," + std::to_string(p.second) + ")";
    }

    // hash
    template< typename T1, typename T2 >
    struct hash< std::pair< T1, T2 > >
    {
        size_t operator()(const std::pair< T1, T2 > & p) const
        {
            std::hash< std::string > hasher;
            return hasher(std::to_string(p));
        }
    };
}


class Board
{
public:
    Board();
    Board(const unsigned int size,
          const char * board);
    Board(const Board & b);
    Board(const BoardData & bd);
    ~Board();

    Board & operator=(const Board & b);
    
    int size() const { return int(size_); }
    
    std::string board_string() const
    {
        std::string buffer_1, buffer_2;
        boost::to_string(black_, buffer_1);
        boost::to_string(white_, buffer_2);
        return buffer_1 + buffer_2;
    }

    std::pair< unsigned int, unsigned int > score() const;

    Board result(bool is_black, const RowColPair & rc);

    bool valid_move(const bool is_black, const RowColPair & rc);
    std::unordered_set< RowColPair > get_valid_moves(const bool is_black);

    bool is_corner(const RowColPair & rc) const;

    int heuristic(const bool is_black, const RowColPair & move);

    friend
    std::ostream & operator<<(std::ostream & cout,
                              const Board & b);
private:
    static const char WHITE;
    static const char BLACK;
    
    static const std::vector< RowColPair > CHECK_PAIRS;

    static unsigned int num_boards;
    static MoveGenUtil * move_generator;
    
    static std::unordered_set< RowColPair > * corners;

    static const int FOUR_BY_FOUR[16];
    static const int SIX_BY_SIX[36];
    static const int EIGHT_BY_EIGHT[64];
    static const int TEN_BY_TEN[100];

    unsigned int size_;
    boost::dynamic_bitset<uint8_t> white_, black_;
    const int * heuristic_board_;

    std::unordered_map< std::pair< bool, RowColPair >, BoardData > results_;

    std::unordered_set< RowColPair > * valid_b_moves_;
    std::unordered_set< RowColPair > * valid_w_moves_;
};

#endif
