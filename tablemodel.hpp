#ifndef TABLEMODEL_HPP
#define TABLEMODEL_HPP

#include "modelindex.hpp"
#include <vector>
#include <string>
#include <map>

using std::vector;
using std::string;
using std::map;

class TableModel
{
    vector<string> columns;
    vector<ulong> rows;
    map<string, map<ulong, string>> data;

public:
    explicit TableModel();
    ~TableModel() = default;

public:
    const vector<string> &getColumns() const { return columns; }
    const vector<ulong> &getRows() const { return rows; }
    string getValue(const ModelIndex &index) const;
    void setValue(const ModelIndex &index, const string &value);
    ModelIndex strToIndex(const string &str) const;
    void calcFuncValues();
    void calcFuncValue(const ModelIndex &index);
};

#endif // TABLEMODEL_HPP