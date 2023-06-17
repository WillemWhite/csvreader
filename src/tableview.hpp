#ifndef TABLEVIEW_HPP
#define TABLEVIEW_HPP

#include "tablemodel.hpp"
#include <memory>

using std::shared_ptr;

class TableView
{
    shared_ptr<TableModel> model;

public:
    explicit TableView(const shared_ptr<TableModel> model = nullptr);
    ~TableView() = default;

public:
    shared_ptr<TableModel> getModel() const { return model; }
    void setModel(shared_ptr<TableModel> model) { this->model = model; }
    const unsigned getMaxCellLen() const;
    void show() const;
    void showCSV() const;
};

#endif // TABLEVIEW_HPP