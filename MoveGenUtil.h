#ifndef MOVE_GEN_UTIL_H
#define MOVE_GEN_UTIL_H

#include <unordered_set>
#include <unordered_map>
#include <string>
#include <boost/dynamic_bitset.hpp>

#include "Common.h"

class MoveGenUtil
{
public:
    MoveGenUtil(const unsigned int size) : size_(size)
    { init_masks(); }
    
    ~MoveGenUtil(){}

    std::unordered_set< RowColPair > get_valid_moves(
        const bool,
        const boost::dynamic_bitset< uint8_t > & b,
        const boost::dynamic_bitset< uint8_t > & w
        );
    
private:
    void init_masks();
    
    const static std::string DIRS[8];
    unsigned int size_;
    std::unordered_map< std::string, boost::dynamic_bitset< uint8_t > > masks_;
};

#endif
