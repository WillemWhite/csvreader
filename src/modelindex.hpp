#ifndef MODELINDEX_HPP
#define MODELINDEX_HPP

#include <string>

struct ModelIndex
{
    std::string column;
    ulong row;

public:
    bool operator==(const ModelIndex &otherIdx)
    {
        return this->column == otherIdx.column && this->row == otherIdx.row;
    }
};

#endif // MODELINDEX_HPP