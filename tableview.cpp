#include "tableview.hpp"
#include <iostream>
#include <iomanip>

using std::cout;
using std::endl;
using std::setiosflags;
using std::ios;
using std::setw;

TableView::TableView(const shared_ptr<TableModel> model)
    : model{model}
{

}

unsigned TableView::getMaxCellLen() const
{
    unsigned maxCellLen = 0;

    if (!model)
        return maxCellLen;

    vector<string> columns{model->getColumns()};
    for (auto &column : columns)
    {
        unsigned columnSize = column.size();
        if (maxCellLen < columnSize)
            maxCellLen = columnSize;
    }

    vector<ulong> rows{model->getRows()};
    for (auto &row : rows)
    {
        unsigned rowSize = std::to_string(row).size();
        if (maxCellLen < rowSize)
            maxCellLen = rowSize;
    }

    for (auto &column : columns)
    {
        for (auto &row : rows)
        {
            string value = model->getValue(ModelIndex{column, row});   
            unsigned valSize = value.size();
            if (maxCellLen < valSize)
                maxCellLen = valSize;
        }
    }

    return maxCellLen;
}

void TableView::show() const
{
    if (!model)
        return;

    unsigned cellLen = getMaxCellLen() + 3;

    cout << setw(cellLen)
         << "";
    vector<string> columns{model->getColumns()};
    for (auto &column : columns)
    {
        cout << setiosflags(ios::left)
             << setw(cellLen)
             << column + ":";
    }
    cout << endl;

    vector<ulong> rows{model->getRows()};
    for (auto &row : rows)
    {
        cout << setiosflags(ios::right)
             << setw(cellLen)
             << std::to_string(row) + ":  "
             << resetiosflags(ios::right);
        for (auto &column : columns)
        {
            string value = model->getValue(ModelIndex{column, row});
            cout << setiosflags(ios::left)
                 << setw(cellLen)
                 << value;
        }
        cout << std::endl;
    }
}

void TableView::showCSV() const
{
    if (!model)
        return;

    vector<string> columns{model->getColumns()};
    for (auto &column : columns)
    {
        cout << "," + column;
    }
    cout << endl;

    vector<ulong> rows{model->getRows()};
    for (auto &row : rows)
    {
        cout << row;
        for (auto &column : columns)
        {
            string value = model->getValue(ModelIndex{column, row});
            cout << "," + value;
        }
        cout << std::endl;
    }
}
