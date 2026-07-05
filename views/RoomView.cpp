#include "RoomView.h"

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QString>
#include <QFrame>
#include <QGraphicsDropShadowEffect>

namespace {
    QString text(QLineEdit* edit) { return edit->text().trimmed(); }
    QString current(QComboBox* box) { return box->currentText(); }

    void applyCardShadow(QWidget *card)
    {
        auto *shadow = new QGraphicsDropShadowEffect(card);
        shadow->setBlurRadius(24);
        shadow->setOffset(0, 4);
        shadow->setColor(QColor(0, 0, 0, 90));
        card->setGraphicsEffect(shadow);
    }
}

RoomView::RoomView(QWidget* parent)
    : QWidget(parent), idEdit(new QLineEdit(this)), typeEdit(new QComboBox(this)), priceEdit(new QDoubleSpinBox(this)),
      statusEdit(new QComboBox(this)), bedsEdit(new QSpinBox(this)), servicesEdit(new QComboBox(this)),
      filterType(new QComboBox(this)), filterStatus(new QComboBox(this)), table(new QTableWidget(this)) {
    typeEdit->addItems({"Standard", "Deluxe", "President"});
    statusEdit->addItems({"Available", "Out of Order", "InUse", "NeedCleaning"});
    filterType->addItems({"All", "Standard", "Deluxe", "President"});
    filterStatus->addItems({"All", "Available", "Booked", "InUse", "NeedCleaning"});
    servicesEdit->addItems({"None", "Bike Rental", "Decoration service", "Laundry service", "Express Room Cleaning", "Extra Bed"});
    priceEdit->setRange(1, 100000000);
    priceEdit->setDecimals(0);
    bedsEdit->setRange(1, 6);

    auto* form = new QFormLayout;
    form->addRow("Room ID", idEdit);
    form->addRow("Type", typeEdit);
    form->addRow("Price", priceEdit);
    form->addRow("Status", statusEdit);
    form->addRow("Beds", bedsEdit);
    form->addRow("Services", servicesEdit);

    auto* actions = new QHBoxLayout;
    auto* addBtn = new QPushButton("Add", this);

    auto* updateBtn = new QPushButton("Update", this);
    auto* deleteBtn = new QPushButton("Delete", this);
    deleteBtn->setObjectName("btnCancel");

    auto* reloadBtn = new QPushButton("Refresh", this);

    actions->addWidget(addBtn);
    actions->addWidget(updateBtn);
    actions->addWidget(deleteBtn);
    actions->addWidget(reloadBtn);

    formCard = new QFrame(this);
    formCard->setObjectName("cardPanel");
    auto* formCardLayout = new QVBoxLayout(formCard);
    formCardLayout->addLayout(form);
    formCardLayout->addLayout(actions);

    auto* filters = new QHBoxLayout;
    auto* filterBtn = new QPushButton("Filter", this);
    filterBtn->setObjectName("btnFilters");
    filters->addWidget(filterType);
    filters->addWidget(filterStatus);
    filters->addWidget(filterBtn);

    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"ID", "Type", "Price", "Status"});
    table->horizontalHeader()->setStretchLastSection(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tableCard = new QFrame(this);
    tableCard->setObjectName("cardPanel");
    auto* tableCardLayout = new QVBoxLayout(tableCard);
    tableCardLayout->addLayout(filters);
    tableCardLayout->addWidget(table);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(formCard);
    layout->addWidget(tableCard);

    applyCardShadow(formCard);
    applyCardShadow(tableCard);
}