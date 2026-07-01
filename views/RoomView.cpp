#include "RoomView.h"

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>
#include <QVBoxLayout>

namespace { std::string text(QLineEdit* edit) { return edit->text().trimmed().toStdString(); } std::string current(QComboBox* box) { return box->currentText().toStdString(); } }

RoomView::RoomView(QWidget* parent)
    : QWidget(parent), idEdit(new QLineEdit(this)), typeEdit(new QComboBox(this)), priceEdit(new QDoubleSpinBox(this)),
      statusEdit(new QComboBox(this)), bedsEdit(new QSpinBox(this)), servicesEdit(new QLineEdit(this)),
      filterType(new QComboBox(this)), filterStatus(new QComboBox(this)), table(new QTableWidget(this)) {
    typeEdit->addItems({"Standard", "Deluxe", "President"});
    statusEdit->addItems({"Available", "Out of Order", "InUse", "NeedCleaning"});
    filterType->addItems({"All", "Standard", "Deluxe", "President"});
    filterStatus->addItems({"All", "Available", "Booked", "InUse", "NeedCleaning"});
    priceEdit->setRange(1, 100000000);
    priceEdit->setDecimals(0);
    bedsEdit->setRange(1, 6);

    auto* form = new QFormLayout;
    form->addRow("ID", idEdit);
    form->addRow("Type", typeEdit);
    form->addRow("Price", priceEdit);
    form->addRow("Status", statusEdit);
    form->addRow("Beds", bedsEdit);
    form->addRow("Services", servicesEdit);

    auto* actions = new QHBoxLayout;
    auto* addBtn = new QPushButton("Add", this);
    auto* updateBtn = new QPushButton("Update", this);
    auto* deleteBtn = new QPushButton("Delete", this);
    auto* reloadBtn = new QPushButton("Refresh", this);
    actions->addWidget(addBtn); actions->addWidget(updateBtn); actions->addWidget(deleteBtn); actions->addWidget(reloadBtn);

    auto* filters = new QHBoxLayout;
    auto* filterBtn = new QPushButton("Filter", this);
    filters->addWidget(filterType); filters->addWidget(filterStatus); filters->addWidget(filterBtn);

    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({"ID", "Type", "Price", "Beds", "Status"});
    table->horizontalHeader()->setStretchLastSection(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    auto* layout = new QVBoxLayout(this);
    layout->addLayout(form); layout->addLayout(actions); layout->addLayout(filters); layout->addWidget(table);
}