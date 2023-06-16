#include "tablemodel.hpp"
#include <algorithm>
#include <iostream>

TableModel::TableModel()
    : columns{}
    , rows{}
    , data{}
{

}

const string TableModel::getValue(const ModelIndex &index) const
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

    processingIndexes.push_back(index);
    for (char opSign : {'+', '-', '*', '/'})
    {
        int idxOpSign = formula.find(opSign, idxEqual);
        if (idxOpSign == string::npos) 
            continue;
        
        string strOp1 = formula.substr(idxEqual+1, idxOpSign - idxEqual-1);
        string strOp2 = formula.substr(idxOpSign+1);

        int numOp1;
        int numOp2;
        try
        {
            numOp1 = operandToIntFromIndex(strOp1, index);
            numOp2 = operandToIntFromIndex(strOp2, index);
        }
        catch(const string &e)
        {
            setValue(index, e);
            processingIndexes.remove(index);
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

        processingIndexes.remove(index);
        return;
    }
}

const ModelIndex TableModel::strToIndex(const string &str) const
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

const int TableModel::operandToIntFromIndex(
    const string &operand, 
    const ModelIndex &index
    )
{
    int num;
    try 
    {
        std::size_t pos;
        num = std::stoi(operand, &pos);
        if (pos != operand.length())
            throw std::invalid_argument{"NOT A NUMBER!"};
    }
    catch (const std::invalid_argument &e)
    {
        // Checking if it's index.
        ModelIndex idxOp;
        try 
        {
            idxOp = strToIndex(operand);
        }
        catch(const string &e)
        {
            throw string{"INVALID OPERAND!"};
        }

        // If index is processing then we can't do anything.
        if (std::find(
                processingIndexes.begin(), 
                processingIndexes.end(), 
                idxOp
                ) != processingIndexes.end()) {
            throw string{"REFERENCE TO CALCULATING IDX!"};
        }

        // Calculating value in operand.
        calcFuncValue(idxOp);
        string valOp = getValue(idxOp);

        // Checking if cell data is integer.
        try
        {
            std::size_t pos;
            num = std::stoi(valOp, &pos);
            if (pos != valOp.length()) {
                throw string{"WRONG VALUES IN CELLS!"};
            }
        }
        catch(const std::invalid_argument &e)
        {
            throw string{"WRONG VALUES IN CELLS!"};
        }
    }
    return num;
}