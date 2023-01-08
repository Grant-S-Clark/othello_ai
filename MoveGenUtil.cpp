#include "MoveGenUtil.h"
#include <iostream> //TEMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
const std::string MoveGenUtil::DIRS[8] = {
    "N", "S", "E", "W", "NE", "NW", "SE", "SW"
};

void print(const boost::dynamic_bitset< uint8_t > & b,
           const unsigned int n)
{
    for (int i = 0; i < n * n; ++i)
    {
        if (b[i])
            std::cout << '1';
        else
            std::cout << '0';

        if ((i + 1) % n == 0)
            std::cout << '\n';
    }
}

void MoveGenUtil::init_masks()
{
    /*
      Temporary North Mask:
      1 1 1 1 1 1
      1 1 1 1 1 1
      1 1 1 1 1 1
      1 1 1 1 1 1
      1 1 1 1 1 1
      0 0 0 0 0 0
    */
    boost::dynamic_bitset< uint8_t > north(size_ * size_);
    north.set();

    for (unsigned int i = 0; i < size_; ++i)
        north[size_ * (size_ - 1) + i] = 0;

    
    /*
      Temporary South Mask:
      0 0 0 0 0 0
      1 1 1 1 1 1
      1 1 1 1 1 1
      1 1 1 1 1 1
      1 1 1 1 1 1
      1 1 1 1 1 1
    */
    
    boost::dynamic_bitset< uint8_t > south(size_ * size_);
    south.set();

    for (unsigned int i = 0; i < size_; ++i)
        south[i] = 0;

    // The above temporary masks are simply for constructing the
    // NE, NW, SE, and SW masks.

    /*
      East Mask:
      0 1 1 1 1 1
      0 1 1 1 1 1
      0 1 1 1 1 1
      0 1 1 1 1 1
      0 1 1 1 1 1
      0 1 1 1 1 1
    */
    
    masks_["E"].resize(size_ * size_);
    masks_["E"].set();

    for (unsigned int i = 0; i < size_ * size_; i += size_)
        masks_["E"][i] = 0;

    /*
      West Mask:
      1 1 1 1 1 0
      1 1 1 1 1 0
      1 1 1 1 1 0
      1 1 1 1 1 0
      1 1 1 1 1 0
      1 1 1 1 1 0
    */

    masks_["W"].resize(size_ * size_);
    masks_["W"].set();

    for (unsigned int i = size_ - 1; i < size_ * size_; i += size_)
        masks_["W"][i] = 0;

    /*
      Northeast Mask.
      0 1 1 1 1 1
      0 1 1 1 1 1
      0 1 1 1 1 1
      0 1 1 1 1 1
      0 1 1 1 1 1
      0 0 0 0 0 0
    */
    masks_["NE"] = north & masks_["E"];

    /*
      Northwest Mask.
      1 1 1 1 1 0
      1 1 1 1 1 0
      1 1 1 1 1 0
      1 1 1 1 1 0
      1 1 1 1 1 0
      0 0 0 0 0 0
    */
    masks_["NW"] = north & masks_["W"];

    /*
      Southeast Mask.
      0 0 0 0 0 0
      0 1 1 1 1 1
      0 1 1 1 1 1
      0 1 1 1 1 1
      0 1 1 1 1 1
      0 1 1 1 1 1
    */
    masks_["SE"] = south & masks_["E"];

    /*
      Soutwest Mask.
      0 0 0 0 0 0
      1 1 1 1 1 0
      1 1 1 1 1 0
      1 1 1 1 1 0
      1 1 1 1 1 0
      1 1 1 1 1 0
    */
    masks_["SW"] = south & masks_["W"];
        
    /*
      REAL North and South Mask:
      1 1 1 1 1 1
      1 1 1 1 1 1
      1 1 1 1 1 1
      1 1 1 1 1 1
      1 1 1 1 1 1
      1 1 1 1 1 1
      
      This is because shifting north and south never results in
      bits running over to another line.
    */
    
    masks_["N"].resize(size_ * size_);
    masks_["N"].set();
    masks_["S"] = masks_["N"];
    
    return;
}

std::unordered_set< RowColPair > MoveGenUtil::get_valid_moves(
    const bool is_black,
    const boost::dynamic_bitset< uint8_t > & b,
    const boost::dynamic_bitset< uint8_t > & w
    )
{
    boost::dynamic_bitset< uint8_t > p, op, available, moves;
    
    if (is_black)
    {
        p = b;
        op = w;
    }
    else
    {
        p = w;
        op = b;
    }

    available = ~(b | w);

    moves.resize(size_ * size_);

    for (unsigned int i = 0; i < 8; ++i)
    {
        std::string d = DIRS[i];

        boost::dynamic_bitset< uint8_t > flood;

        //Set the initial flood
        switch (i)
        {
            case 0: // N
                flood = ((p >> size_) & masks_[d]) & op;
                break;
            case 1: // S
                flood = ((p << size_) & masks_[d]) & op;
                break;
            case 2: // E
                flood = ((p << 1) & masks_[d]) & op;
                break;
            case 3: // W
                flood = ((p >> 1) & masks_[d]) & op;
                break;
            case 4: // NE
                flood = (((p >> size_) << 1) & masks_[d]) & op;
                break;
            case 5: // NW
                flood = (((p >> size_) >> 1) & masks_[d]) & op;
                break;
            case 6: // SE
                flood = (((p << size_) << 1) & masks_[d]) & op;
                break;
            case 7: // SW
                flood = (((p << size_) >> 1) & masks_[d]) & op;
        }

        // Adjust flood n - 1 times.
        for (unsigned int j = 0; j < size_ - 1; ++j)
        {
            switch (i)
            {
                case 0: // N
                    flood |= ((flood >> size_) & masks_[d]) & op;
                    break;
                case 1: // S
                    flood |= ((flood << size_) & masks_[d]) & op;
                    break;
                case 2: // E
                    flood |= ((flood << 1) & masks_[d]) & op;
                    break;
                case 3: // W
                    flood |= ((flood >> 1) & masks_[d]) & op;
                    break;
                case 4: // NE
                    flood |= (((flood >> size_) << 1) & masks_[d]) & op;
                    break;
                case 5: // NW
                    flood |= (((flood >> size_) >> 1) & masks_[d]) & op;
                    break;
                case 6: // SE
                    flood |= (((flood << size_) << 1) & masks_[d]) & op;
                    break;
                case 7: // SW
                    flood |= (((flood << size_) >> 1) & masks_[d]) & op;
            }

            
        }

        // Update list of moves while also making sure those
        // moves are on available spaces.
        switch (i)
        {
            case 0: // N
                moves |= ((flood >> size_) & masks_[d]) & available;
                break;
            case 1: // S
                moves |= ((flood << size_) & masks_[d]) & available;
                break;
            case 2: // E
                moves |= ((flood << 1) & masks_[d]) & available;
                break;
            case 3: // W
                moves |= ((flood >> 1) & masks_[d]) & available;
                break;
            case 4: // NE
                moves |= (((flood >> size_) << 1) & masks_[d]) & available;
                break;
            case 5: // NW
                moves |= (((flood >> size_) >> 1) & masks_[d]) & available;
                break;
            case 6: // SE
                moves |= (((flood << size_) << 1) & masks_[d]) & available;
                break;
            case 7: // SW
                moves |= (((flood << size_) >> 1) & masks_[d]) & available;
        }
    }

    // Store found moves into a set.
    std::unordered_set< RowColPair > ret;

    for (int i = 0; i < size_ * size_; ++i)
        if (moves[i])
            ret.insert({ i / int(size_), i % int(size_) });
    
    return ret;
}
