#include "tablemodel.hpp"
#include "tableview.hpp"
#include <iostream>
#include <algorithm>
#include <fstream>

using std::string;
using std::cout;
using std::endl;
using std::make_shared;

int main(int argc, char *argv[])
{
    if (argc < 2) {
        cout << "Please, enter file name after program name like: " << endl;
        cout << "$ ./csvreader file.csv" << endl;
        return -1;
    }

    string fileName{argv[1]};

    shared_ptr<TableModel> model{make_shared<TableModel>(TableModel{})};
    if (!model->loadFromFile(fileName)) {
        cout << "Error while loading file. Exit..." << endl;
        return -1;
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

    return 0;
}