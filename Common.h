#ifndef COMMON_H
#define COMMON_H

struct RowColPair
{
    int r;
    int c;

    bool operator==(const RowColPair & pair) const
    { return r == pair.r && c == pair.c; }
};

namespace std
{
    inline
    std::string to_string(const RowColPair & rc)
    {
        return std::to_string(rc.r) + "," + std::to_string(rc.c);
    }

    inline
    std::string to_string(bool b)
    {
        if (b)
            return "True";
        return "False";
    }

    template<>
    struct hash< RowColPair >
    {
        size_t operator()(const RowColPair & rc) const
        {
            std::hash< std::string > hasher;
            return hasher(std::to_string(rc));
        }
    };
}

#endif
