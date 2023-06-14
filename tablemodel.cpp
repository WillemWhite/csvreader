#include "tablemodel.hpp"
#include <algorithm>
#include <iostream>

TableModel::TableModel()
    : columns{}
    , rows{}
    , data{}
{

}

string TableModel::getValue(const ModelIndex &index) const
{
    bool colExists = std::find(columns.begin(), columns.end(), index.column) 
                     != columns.end();
    bool rowExists = std::find(rows.begin(), rows.end(), index.row) 
                     != rows.end();

    if (colExists && rowExists) {
        string value;
        try
        {
            value = data.at(index.column).at(index.row);
        }
        catch(const std::out_of_range& e)
        {
            value = "";
        }
        return value;
    }
    
    return string("INVALID CELL!");
}

void TableModel::setValue(const ModelIndex &index, const string &value)
{
    // If column or row is new then add it to corresponding vector.
    if (std::find(columns.begin(), columns.end(), index.column) == columns.end())
        columns.push_back(index.column);
    if (std::find(rows.begin(), rows.end(), index.row) == rows.end())
        rows.push_back(index.row);

    data[index.column][index.row] = value;
}

ModelIndex TableModel::strToIndex(const string &str) const
{
    string strReduced;
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] != ' ')
            strReduced += str[i];
    }

    for (const auto &column : columns)
    {
        int pos = strReduced.find(column);
        if (pos != 0)
            continue;

        pos += column.length();
        for (const auto &row : rows)
        {
            if (std::to_string(row) == strReduced.substr(pos))
                return {column, row};
        }
    }

    throw string{"INVALID INDEX!"};
}

void TableModel::calcFuncValues()
{
    for (const auto &column : columns)
    {
        for (const auto &row : rows)
        {
            calcFuncValue(ModelIndex{column, row});
        }
    }
}

void TableModel::calcFuncValue(const ModelIndex &index)
{
    string formula = getValue(index);
    int idxEqual = formula.find("=");
    if (idxEqual == string::npos)
        return;

    for (char opSign : {'+', '-', '*', '/'})
    {
        int idxOpSign = formula.find(opSign, idxEqual);
        if (idxOpSign == string::npos) 
            continue;
        
        string strOp1 = formula.substr(idxEqual+1, idxOpSign - idxEqual-1);
        string strOp2 = formula.substr(idxOpSign+1);

        ModelIndex idxOp1;
        ModelIndex idxOp2;
        try 
        {
            idxOp1 = strToIndex(strOp1);
            idxOp2 = strToIndex(strOp2);
        }
        catch(const string &e)
        {
            setValue(index, e);
            return;
        }

        // If exists index of current cell in formula then error.
        if (idxOp1.column == index.column && idxOp1.row == index.row
            || idxOp2.column == index.column && idxOp2.row == index.row) {
            setValue(index, "INVALID INDEX!");
            return;
        }
        
        // Calculating values in operands.
        calcFuncValue(idxOp1);
        calcFuncValue(idxOp2);

        string valOp1 = getValue(idxOp1);
        string valOp2 = getValue(idxOp2);
        
        // If operand have an error then this cell have this error too.
        if (valOp1 == "INVALID INDEX!" || valOp2 == "INVALID INDEX!") {
            setValue(index, "INVALID INDEX!");
            return;
        }
        if (valOp1 == "WRONG VALUES IN CELLS!" || valOp2 == "WRONG VALUES IN CELLS!") {
            setValue(index, "WRONG VALUES IN CELLS!");
            return;
        }
        if (valOp1 == "DIVISION BY ZERO!" || valOp2 == "DIVISION BY ZERO!") {
            setValue(index, "DIVISION BY ZERO!");
            return;
        }
        
        // Checking if cell data is integer.
        std::size_t pos{};
        int numOp1;
        int numOp2;
        try
        {
            numOp1 = std::stoi(valOp1, &pos);
            if (pos != valOp1.length()) {
                setValue(index, "WRONG VALUES IN CELLS!");
                return;
            }
            numOp2 = std::stoi(valOp2, &pos);
            if (pos != valOp2.length()) {
                setValue(index, "WRONG VALUES IN CELLS!");
                return;
            }
        }
        catch(const std::invalid_argument &e)
        {
            setValue(index, "WRONG VALUES IN CELLS!");
            return;
        }

        switch (opSign)
        {
        case '+':
            setValue(index, std::to_string(numOp1 + numOp2));
            break;
        case '-':
            setValue(index, std::to_string(numOp1 - numOp2));
            break;
        case '*':
            setValue(index, std::to_string(numOp1 * numOp2));
            break;
        case '/':
            if (numOp2 != 0)
                setValue(index, std::to_string(numOp1 / numOp2));
            else
                setValue(index, "DIVISION BY ZERO!");
            break;
        }
    }
}