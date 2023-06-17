#include "tablemodel.hpp"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <memory>

using std::cout, std::endl;
using std::to_string;
using std::size_t, std::stoi;
using std::ifstream, std::getline;
using std::shared_ptr, std::make_shared;

TableModel::TableModel()
    : columns{}
    , rows{}
    , data{}
{

}

const bool TableModel::loadFromFile(const string &fileName)
{
    if (fileName.find(".csv") == string::npos) {
        cout << "Wrong format file! Please, enter <*.csv> file." << endl;
        return false;
    }

    ifstream in;
    in.open(fileName);
    if (!in.is_open()) {
        cout << "Can't open file." << endl;
        return false;
    }
    cout << "File succesfully opened!" << endl;

    string line;
    int idxRow = -1;
    int idxCol = 0;
    vector<string> columns{};
    vector<ulong> rows{};
    while (getline(in, line))
    {
        int pos = 0;

        if (line.find(",", 0) == string::npos)
            break;

        if (idxRow == -1) {
            pos = line.find(",", pos)+1;
            if (pos != 1) {
                cout << "First symbol in file must be ','." << endl;
                in.close();
                return false;
            }
            while (pos != line.length())
            {
                string column;
                if (line.find(",", pos) != string::npos) {
                    column = line.substr(pos, line.find(",", pos) - pos);
                    pos = line.find(",", pos)+1;
                } else {
                    column = line.substr(pos);
                    pos = line.length();
                }

                if (std::find(columns.begin(), columns.end(), column) != columns.end()) {
                    cout << "Columns in table not unique." << endl;
                    in.close();
                    return false;
                }
                columns.push_back(column);
            }
            idxRow++;
            continue;
        }

        pos = line.find(",", pos)+1;
        string strRow = line.substr(0, pos-1);
        ulong row;
        size_t p{};
        try
        {
            row = stoi(strRow, &p);
            if (p != strRow.length() || row < 1) {
                cout << "Row must be positive integer." << endl;
                in.close();
                return false;
            }
        }
        catch(const std::invalid_argument &e)
        {
            cout << "Row must be positive integer." << endl;
            in.close();
            return false;
        }

        if (std::find(rows.begin(), rows.end(), row) != rows.end()) {
            cout << "Rows in table not unique." << endl;
            in.close();
            return false;
        }
        rows.push_back(row);

        while (pos != line.length())
        {
            string cellData;
            if (line.find(",", pos) != string::npos) {
                cellData = line.substr(pos, line.find(",", pos) - pos);
                pos = line.find(",", pos)+1;
            } else {
                cellData = line.substr(pos);
                pos = line.length();
            }
            setValue({columns[idxCol], rows[idxRow]}, cellData);
            idxCol++;
        }
        if (idxCol != columns.size()) {
            cout << "Count of columns in row isn't match with announced." << endl;
            in.close();
            return false;
        }

        idxRow++;
        idxCol = 0;
    }

    in.close();
    
    return true;
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
            setValue(index, to_string(numOp1 + numOp2));
            break;
        case '-':
            setValue(index, to_string(numOp1 - numOp2));
            break;
        case '*':
            setValue(index, to_string(numOp1 * numOp2));
            break;
        case '/':
            if (numOp2 != 0)
                setValue(index, to_string(numOp1 / numOp2));
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
            if (to_string(row) == strReduced.substr(pos))
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
        size_t pos;
        num = stoi(operand, &pos);
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
            size_t pos;
            num = stoi(valOp, &pos);
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