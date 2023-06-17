#ifndef TABLEMODEL_HPP
#define TABLEMODEL_HPP

#include "modelindex.hpp"
#include <vector>
#include <string>
#include <map>
#include <list>

using std::vector;
using std::string;
using std::map;
using std::list;

class TableModel
{
    vector<string> columns;
    vector<ulong> rows;
    map<string, map<ulong, string>> data;
    list<ModelIndex> processingIndexes;

public:
    explicit TableModel();
    ~TableModel() = default;

public:
    const bool loadFromFile(const string &fileName);
    const vector<string> &getColumns() const { return columns; }
    const vector<ulong> &getRows() const { return rows; }
    const string getValue(const ModelIndex &index) const;
    void setValue(const ModelIndex &index, const string &value);
    void calcFuncValues();
    void calcFuncValue(const ModelIndex &index);
    const int operandToIntFromIndex(const string &operand, const ModelIndex &index);
    const ModelIndex strToIndex(const string &str) const;
};

#endif // TABLEMODEL_HPP