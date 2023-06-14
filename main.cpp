#include "tablemodel.hpp"
#include "tableview.hpp"
#include <iostream>
#include <algorithm>
#include <fstream>

using std::string;
using std::cout;
using std::endl;
using std::ifstream;
using std::getline;
using std::make_shared;

int main(int argc, char *argv[])
{
    if (argc < 2) {
        cout << "Please, enter file name after program name like: " << endl;
        cout << "$ ./csvreader file.csv" << endl;
        return -1;
    }

    string filename{argv[1]};
    if (filename.find(".csv") == string::npos) {
        cout << "Wrong format file! Please, enter <*.csv> file." << endl;
        return -1;
    }

    ifstream in;
    in.open(filename);
    if (!in.is_open()) {
        cout << "Can't open file." << endl;
        return -1;
    }
    cout << "File succesfully opened!" << endl;

    string line;
    int idxRow = -1;
    int idxCol = 0;
    vector<string> columns{};
    vector<ulong> rows{};
    shared_ptr<TableModel> model{make_shared<TableModel>(TableModel{})};
    while (getline(in, line))
    {
        int pos = 0;

        if (line.find(",", 0) == string::npos)
            break;

        if (idxRow == -1) {
            pos = line.find(",", pos)+1;
            if (pos != 1) {
                cout << "First symbol in file must be ','." << endl;
                return -1;
            }
            while (pos != line.length())
            {
                string column;
                if (line.find(",", pos) != string::npos) {
                    column = line.substr(pos, line.find(",", pos) - pos);
                    pos = line.find(",", pos)+1;
                }
                else {
                    column = line.substr(pos);
                    pos = line.length();
                }

                if (std::find(columns.begin(), columns.end(), column) != columns.end()) {
                    cout << "Columns in table not unique." << endl;
                    return -1;
                }
                columns.push_back(column);
            }
            idxRow++;
            continue;
        }

        pos = line.find(",", pos)+1;
        string strRow = line.substr(0, pos-1);
        ulong row;
        std::size_t p{};
        try
        {
            row = std::stoi(strRow, &p);
            if (p != strRow.length() || row < 1) {
                cout << "Row must be positive integer." << endl;
                return -1;
            }
        }
        catch(const std::invalid_argument &e)
        {
            cout << "Row must be positive integer." << endl;
            return -1;
        }
        rows.push_back(row);

        while (pos != line.length())
        {
            string cellData;
            if (line.find(",", pos) != string::npos) {
                cellData = line.substr(pos, line.find(",", pos) - pos);
                pos = line.find(",", pos)+1;
            }
            else {
                cellData = line.substr(pos);
                pos = line.length();
            }
            model->setValue({columns[idxCol], rows[idxRow]}, cellData);
            idxCol++;
        }
        if (idxCol != columns.size()) {
            cout << "Count of columns in row isn't match with announced." << endl;
            return -1;
        }

        idxRow++;
        idxCol = 0;
    }

    TableView view(model);
    cout << "Current table in file:" << endl;
    view.show();
    cout << "Current table in CSV format in file:" << endl;
    view.showCSV();
    cout << endl;

    model->calcFuncValues();
    cout << "Table with calculated functions:" << endl;
    view.show();
    cout << "Table with calculated functions in CSV format:" << endl;
    view.showCSV();

    in.close();

    return 0;
}