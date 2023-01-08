#include "Board.h"

const char Board::WHITE('W');
const char Board::BLACK('B');

const std::vector< RowColPair > Board::CHECK_PAIRS(
    {
    {1, 0}, {-1, 0}, {0, 1}, {0, -1},
    {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    }
);

std::unordered_set< RowColPair > * Board::corners(nullptr);
MoveGenUtil * Board::move_generator(nullptr);

unsigned int Board::num_boards(0);

const int Board::FOUR_BY_FOUR[16] = {
    100, 10, 10, 100,
     10,  0,  0,  10,
     10,  0,  0,  10,
    100, 10, 10, 100
};

const int Board::SIX_BY_SIX[36] = {
    100, -20,   8,   8, -20, 100,
    -20,  -6,   4,   4,  -6, -20,
      8,   4,   0,   0,   4,   8,
      8,   4,   0,   0,   4,   8,
    -20,  -6,   4,   4,  -6, -20,
    100, -20,   8,   8, -20, 100
};

const int Board::EIGHT_BY_EIGHT[64] = {
    100, -10,   8,   6,   6,   8, -10, 100,
    -10, -25,  -4,  -4,  -4,  -4, -25, -10,
      8,  -4,   6,   4,   4,   6,  -4,   8,
      6,  -4,   4,   0,   0,   4,  -4,   6,
      6,  -4,   4,   0,   0,   4,  -4,   6,
      8,  -4,   6,   4,   4,   6,  -4,   8,
    -10, -25,  -4,  -4,  -4,  -4, -25, -10,
    100, -10,   8,   6,   6,   8, -10, 100
};

const int Board::TEN_BY_TEN[100] = {
    150, -16,  14,  12,  10,  10,  12,  14, -16, 150,
    -16, -35,  -6,  -6,  -6,  -6,  -6,  -6, -35, -16,
     14,  -6, -25,  -4,  -4,  -4,  -4, -25,  -6,  14,
     12,  -6,  -4,  6,    4,   4,  6,   -4,  -6,  12,
     10,  -6,  -4,  4,    0,   0,  4,   -4,  -6,  10,
     10,  -6,  -4,  4,    0,   0,  4,   -4,  -6,  10,
     12,  -6,  -4,  6,    4,   4,  6,   -4,  -6,  12,
     14,  -6, -25, -4,   -4,  -4,  -4, -25,  -6,  14,
    -16, -35,  -6, -6,   -6,  -6,  -6,  -6, -35, -16,
    150, -16,  14, 12,   10,  10,  12,  14, -16, 150
};

Board::Board() :
    size_(0),
    white_(0),
    black_(0),
    heuristic_board_(nullptr),
    valid_b_moves_(nullptr),
    valid_w_moves_(nullptr)
{
    ++num_boards;
}

class InvalidBoardSizeError{};

Board::Board(const unsigned int size,
             const char * board) :
    size_(size),
    heuristic_board_(nullptr),
    valid_b_moves_(nullptr),
    valid_w_moves_(nullptr)
{
    if (size_ != 4 && size_ != 6 && size_ != 8 && size_ != 10)
        throw InvalidBoardSizeError();
    
    if (num_boards == 0)
    {
        corners = new std::unordered_set< RowColPair >(
            { {0, 0},
              {0, int(size_) - 1},
              {int(size_) - 1, 0},
              {int(size_) - 1, int(size_) - 1} }
            );

        move_generator = new MoveGenUtil(size_);
    }

    ++num_boards;
    
    switch (size_)
    {
        case 4:
            heuristic_board_ = FOUR_BY_FOUR;
            break;
        case 6:
            heuristic_board_ = SIX_BY_SIX;
            break;
        case 8:
            heuristic_board_ = EIGHT_BY_EIGHT;
            break;
        case 10:
            heuristic_board_ = TEN_BY_TEN;
            break;
        default:
            class InvalidBoardSizeError{};
            throw InvalidBoardSizeError();
    }

    white_.resize(size_ * size_);
    black_.resize(size_ * size_);

    // Assuming that the board array sent in is of dimensions
    // size x size
    for (unsigned int i = 0; i < size_; ++i)
    {
        for (unsigned int j = 0; j < size_; ++j)
        {
            if (board[(i * size_) + j] == BLACK)
                black_[(i * size_) + j] = 1;
            else if (board[(i * size_) + j] == WHITE)
                white_[(i * size_) + j] = 1;
        }
    }
    
    return;
}

Board::Board(const Board & b) :
    black_(b.black_),
    white_(b.white_),
    size_(b.size_),
    heuristic_board_(b.heuristic_board_),
    valid_b_moves_(nullptr),
    valid_w_moves_(nullptr)
{
    ++num_boards;
}

Board::Board(const BoardData & bd) :
    black_(bd.black),
    white_(bd.white),
    size_(bd.size),
    heuristic_board_(nullptr),
    valid_b_moves_(nullptr),
    valid_w_moves_(nullptr)
{
    ++num_boards;
    
    switch (size_)
    {
        case 4:
            heuristic_board_ = FOUR_BY_FOUR;
            break;
        case 6:
            heuristic_board_ = SIX_BY_SIX;
            break;
        case 8:
            heuristic_board_ = EIGHT_BY_EIGHT;
            break;
        case 10:
            heuristic_board_ = TEN_BY_TEN;
            break;
        default:
            class InvalidBoardSizeError{};
            throw InvalidBoardSizeError();
    }

    return;
}

Board::~Board()
{
    --num_boards;
    
    if (num_boards == 0)
    {
        if (corners != nullptr)
        {
            delete corners;
            corners = nullptr;
        }

        if (move_generator != nullptr)
        {
            delete move_generator;
            move_generator = nullptr;
        }
    }

    if (valid_b_moves_ != nullptr)
        delete valid_b_moves_;
    
    if (valid_w_moves_ != nullptr)
        delete valid_w_moves_;
    
    return;
}


Board & Board::operator=(const Board & b)
{
    black_ = b.black_;
    white_ = b.white_;
    size_ = b.size_;
    heuristic_board_ = b.heuristic_board_;

    if (valid_b_moves_ != nullptr)
    {
        delete valid_b_moves_;
        valid_b_moves_ = nullptr;
    }

    if (valid_w_moves_ != nullptr)
    {
        delete valid_w_moves_;
        valid_w_moves_ = nullptr;
    }

    results_.clear();
    
    return *this;
}


std::pair< unsigned int, unsigned int > Board::score() const
{
    unsigned int black = 0, white = 0;
    
    unsigned int n = size_ * size_;
    for (unsigned int i = 0; i < n; ++i)
    {
        if (black_[i])
            black += 1;
        else if (white_[i])
            white += 1;
    }
    
    return { black, white };
}


// NOTE: This function assumes that rc is a valid move.
Board Board::result(bool is_black, const RowColPair & move)
{
    std::pair< bool , RowColPair > key = {is_black, move};
    
    if (results_.find(key) != results_.end())
        return Board(results_[key]);
    
    Board ret(*this);

    int r = move.r, c = move.c;
    
    int move_index = move.r * size_ + move.c;

    if (is_black)
        ret.black_[move_index] = 1;
    else
        ret.white_[move_index] = 1;

    for (const RowColPair & rc : CHECK_PAIRS)
    {
        int i = rc.r, j = rc.c, k = 1;

        while (0 <= r + i * k && r + i * k < size_ &&
               0 <= c + j * k && c + j * k < size_)
        {
            int index = (r + i * k) * size_ + (c + j * k);
            if (!ret.black_[index] && !ret.white_[index])
                break;
            else if ((ret.black_[index] && is_black) || (ret.white_[index] && !is_black))
            {
                --k;
                while (k >= 1)
                {
                    index = (r + i * k) * size_ + (c + j * k);
                    ret.black_.flip(index);
                    ret.white_.flip(index);
                    --k;
                }
                break;
            }
            else
                ++k;
        }
    }

    results_[key] = {ret.black_, ret.white_, ret.size_};

    return ret;
}

bool Board::valid_move(const bool is_black, const RowColPair & rc)
{
    if (is_black)
    {
        if (valid_b_moves_ == nullptr)
            valid_b_moves_ = new std::unordered_set< RowColPair >(move_generator->get_valid_moves(is_black, black_, white_));
        return valid_b_moves_->find(rc) != valid_b_moves_->end();
    }

    if (valid_w_moves_ == nullptr)
        valid_w_moves_ = new std::unordered_set< RowColPair >(move_generator->get_valid_moves(is_black, black_, white_));
    return valid_w_moves_->find(rc) != valid_w_moves_->end();
}

std::unordered_set< RowColPair > Board::get_valid_moves(const bool is_black)
{
    if (is_black)
    {
        if (valid_b_moves_ == nullptr)
            valid_b_moves_ = new std::unordered_set< RowColPair >(move_generator->get_valid_moves(is_black, black_, white_));
        return *valid_b_moves_;
    }

    if (valid_w_moves_ == nullptr)
        valid_w_moves_ = new std::unordered_set< RowColPair >(move_generator->get_valid_moves(is_black, black_, white_));
    return *valid_w_moves_;
}


bool Board::is_corner(const RowColPair & rc) const
{
    return corners->find(rc) != corners->end();
}


int Board::heuristic(const bool is_black,
                     const RowColPair & move)
{
    /*
      This function returns a 'value' associated with a move.
      The move that is made is determined by the black_move boolean,
      i.e. whether a black or white piece is placed.
      This value is either determined by whether it is in an
      advantageous positon, and if it is not, returns a sum of
      the differences between the black and white piece counts before
      and after the move. Always calculated as white being the 'Max'
      value, i.e. bigger values are good for the white pieces, and
      lower values are better for black pieces.
        
      TLDR: Big number   = Good for white
            Small number = Good for Black
    */
    int ret = 0;

    int r = move.r, c = move.c;

    Board result_board = result(is_black, move);
    std::pair< unsigned int, unsigned int > score = result_board.score();

    // Piece ratio heuristic
    ret += (score.first - score.second);
    
    // Placement heuristic
    if (is_black)
        ret -= heuristic_board_[size_ * r + c];
    else
        ret += heuristic_board_[size_ * r + c];

    return ret;
}

std::ostream & operator<<(std::ostream & cout,
                          const Board & b)
{   
    unsigned int n = b.size_ * b.size_;
    
    for (int i = 0; i < b.size_; ++i)
        cout << "+-";
    cout << "+\n";
    
    for (int i = 0; i < n; ++i)
    {
        cout << '|';
        if (b.white_[i])
            cout << b.WHITE;
        else if (b.black_[i])
            cout << b.BLACK;
        else
            cout << ' ';
        
        if ((i + 1) % b.size_ == 0)
        {
            cout << "|\n";
            for (int j = 0; j < b.size_; ++j)
                cout << "+-";
            cout << "+\n";
        }
    }
    
    return cout;
}
