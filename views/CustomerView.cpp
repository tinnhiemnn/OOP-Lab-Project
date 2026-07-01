#include "CustomerView.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

namespace { std::string text(QLineEdit* edit) { return edit->text().trimmed().toStdString(); } }

CustomerView::CustomerView(QWidget* parent)
    : QWidget(parent), idEdit(new QLineEdit(this)), nameEdit(new QLineEdit(this)), emailEdit(new QLineEdit(this)),
      phoneEdit(new QLineEdit(this)), searchEdit(new QLineEdit(this)), table(new QTableWidget(this)) {
    auto* form = new QFormLayout;
    form->addRow("ID", idEdit);
    form->addRow("Name", nameEdit);
    form->addRow("Email", emailEdit);
    form->addRow("Phone", phoneEdit);

    auto* actions = new QHBoxLayout;
    auto* addBtn = new QPushButton("Add", this);
    auto* updateBtn = new QPushButton("Update", this);
    auto* deleteBtn = new QPushButton("Delete", this);
    auto* reloadBtn = new QPushButton("Refresh", this);
    actions->addWidget(addBtn);
    actions->addWidget(updateBtn);
    actions->addWidget(deleteBtn);
    actions->addWidget(reloadBtn);

    auto* searching = new QHBoxLayout;
    auto* searchBtn = new QPushButton("Search", this);
    searchEdit->setPlaceholderText("Search customers");
    searching->addWidget(searchEdit);
    searching->addWidget(searchBtn);

    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"ID", "Name", "Email", "Phone"});
    table->horizontalHeader()->setStretchLastSection(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    auto* layout = new QVBoxLayout(this);
    layout->addLayout(form);
    layout->addLayout(actions);
    layout->addLayout(searching);
    layout->addWidget(table);
}